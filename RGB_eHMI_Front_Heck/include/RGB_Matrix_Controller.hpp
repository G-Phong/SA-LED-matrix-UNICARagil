#ifndef RGB_MATRIX_CONTROLLER_HPP
#define RGB_MATRIX_CONTROLLER_HPP

// If not run on ARM-Powered Hardware (assuming Raspberry Pi here), we want to enter DUMMY_MODE
#ifndef __ARM_ARCH
    #define DUMMY_MODE
#endif

#include <asoa/core/runtime.hpp>
#include <asoa/driver/rtps.h>
#include <asoa/core/task.hpp>

// hzeller-Library + Tools
#include "led-matrix.h"
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <cstring>

#ifndef DUMMY_MODE
    //Including files for the image-viewer -> must be installed on the Raspi
    #include <Magick++.h>
    #include <magick/image.h>
#endif

#include <chrono>
#include <thread>
#include "pixel-mapper.h"
#include "content-streamer.h"
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <map>
#include <string>
#include <vector>

// To use this image viewer, first get image-magick development files
// $ sudo apt-get install libgraphicsmagick++-dev libwebp-dev -y
// CMakeLists.txt muss die GraphicsMagick++ Library inkludieren in die Executables! -> Flags linken!
//
// Then compile with
// $ make led-image-viewer
// $ sudo apt install graphicsmagick-libmagick-dev-compat -> StackOverFlow -> lieber nicht machen

using rgb_matrix::Canvas;
using rgb_matrix::FrameCanvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::StreamReader;


  typedef int64_t tmillis_t;
  static const tmillis_t distant_future = (1LL<<40); //equals value "1099511627776"

  struct ImageParams {
    ImageParams() : anim_duration_ms(distant_future), wait_ms(1500),
                    anim_delay_ms(-1), loops(-1), vsync_multiple(1) {}
    tmillis_t anim_duration_ms;  // If this is an animation, duration to show.
    tmillis_t wait_ms;           // Regular image: duration to show.
    tmillis_t anim_delay_ms;     // Animation delay override.
    int loops;
    int vsync_multiple;
  };

  struct FileInfo {
    ImageParams params;      // Each file might have specific timing settings
    bool is_multi_frame;
    rgb_matrix::StreamIO *content_stream;
  };

  volatile bool interrupt_received = false;
  static void InterruptHandler(int signo) { //statische Funktion in einer Klasse -> geht nicht
    interrupt_received = true;
  }

  static tmillis_t GetTimeInMillis() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
  }

  static void SleepMillis(tmillis_t milli_seconds) {
    if (milli_seconds <= 0) return;
    struct timespec ts;
    ts.tv_sec = milli_seconds / 1000;
    ts.tv_nsec = (milli_seconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
  }

  static void StoreInStream(const Magick::Image &img, int delay_time_us,
                            bool do_center,
                            rgb_matrix::FrameCanvas *scratch,
                            rgb_matrix::StreamWriter *output) {
    scratch->Clear();
    const int x_offset = do_center ? (scratch->width() - img.columns()) / 2 : 0;
    const int y_offset = do_center ? (scratch->height() - img.rows()) / 2 : 0;
    for (size_t y = 0; y < img.rows(); ++y) {
      for (size_t x = 0; x < img.columns(); ++x) {
        const Magick::Color &c = img.pixelColor(x, y);
        if (c.alphaQuantum() < 256) {
          scratch->SetPixel(x + x_offset, y + y_offset,
                            ScaleQuantumToChar(c.redQuantum()),
                            ScaleQuantumToChar(c.greenQuantum()),
                            ScaleQuantumToChar(c.blueQuantum()));
        }
      }
    }
    output->Stream(*scratch, delay_time_us);
  }

  static void CopyStream(rgb_matrix::StreamReader *r,
                         rgb_matrix::StreamWriter *w,
                         rgb_matrix::FrameCanvas *scratch) {
    uint32_t delay_us;
    while (r->GetNext(scratch, &delay_us)) {
      w->Stream(*scratch, delay_us);
    }
  }

  // Load still image or animation.
  // Scale, so that it fits in "width" and "height" and store in "result".
  static bool LoadImageAndScale(const char *filename,
                                int target_width, int target_height,
                                bool fill_width, bool fill_height,
                                std::vector<Magick::Image> *result,
                                std::string *err_msg) {
    std::vector<Magick::Image> frames;
    try {
      readImages(&frames, filename);
    } catch (std::exception& e) {
      if (e.what()) *err_msg = e.what();
      return false;
    }
    if (frames.size() == 0) {
      fprintf(stderr, "No image found.");
      return false;
    }

    // Put together the animation from single frames. GIFs can have nasty
    // disposal modes, but they are handled nicely by coalesceImages()
    if (frames.size() > 1) {
      Magick::coalesceImages(result, frames.begin(), frames.end());
    } else {
      result->push_back(frames[0]);   // just a single still image.
    }

    const int img_width = (*result)[0].columns();
    const int img_height = (*result)[0].rows();
    const float width_fraction = (float)target_width / img_width;
    const float height_fraction = (float)target_height / img_height;
    if (fill_width && fill_height) {
      // Scrolling diagonally. Fill as much as we can get in available space.
      // Largest scale fraction determines that.
      const float larger_fraction = (width_fraction > height_fraction)
        ? width_fraction
        : height_fraction;
      target_width = (int) roundf(larger_fraction * img_width);
      target_height = (int) roundf(larger_fraction * img_height);
    }
    else if (fill_height) {
      // Horizontal scrolling: Make things fit in vertical space.
      // While the height constraint stays the same, we can expand to full
      // width as we scroll along that axis.
      target_width = (int) roundf(height_fraction * img_width);
    }
    else if (fill_width) {
      // dito, vertical. Make things fit in horizontal space.
      target_height = (int) roundf(width_fraction * img_height);
    }

    for (size_t i = 0; i < result->size(); ++i) {
      (*result)[i].scale(Magick::Geometry(target_width, target_height));
    }

    return true;
  }

  void DisplayAnimation(const FileInfo *file,
                        RGBMatrix *matrix, FrameCanvas *offscreen_canvas) {
    const tmillis_t duration_ms = (file->is_multi_frame
                                   ? file->params.anim_duration_ms
                                   : file->params.wait_ms);
    rgb_matrix::StreamReader reader(file->content_stream);
    int loops = file->params.loops;
    const tmillis_t end_time_ms = GetTimeInMillis() + duration_ms;
    const tmillis_t override_anim_delay = file->params.anim_delay_ms;
    for (int k = 0;
         (loops < 0 || k < loops)
           && !interrupt_received
           && GetTimeInMillis() < end_time_ms;
         ++k) {
      uint32_t delay_us = 0;
      while (!interrupt_received && GetTimeInMillis() <= end_time_ms
             && reader.GetNext(offscreen_canvas, &delay_us)) {
        const tmillis_t anim_delay_ms =
          override_anim_delay >= 0 ? override_anim_delay : delay_us / 1000;
        const tmillis_t start_wait_ms = GetTimeInMillis();
        offscreen_canvas = matrix->SwapOnVSync(offscreen_canvas,
                                               file->params.vsync_multiple);
        const tmillis_t time_already_spent = GetTimeInMillis() - start_wait_ms;
        SleepMillis(anim_delay_ms - time_already_spent);
      }
      reader.Rewind();
    }
  }

  static int usage(const char *progname) {
    //
    return 1;
  }

//TODO: Funktion in die Klassen "ziehen"
//TODO: Virtual Class erstellen -> hzellerController und DummyRGBController sollen von RGBMatrixController erben
class RGBMatrixController{
public:
  virtual void RGB_Thread() = 0;

  virtual void bild_anzeigen(std::string s) = 0;

  virtual void setIsDisplayActive(bool value) = 0;

  virtual void setFilepath(std::string value) = 0;

};

// RGB Controller Unterklasse "hzeller" -> zeigt wirklich was an
class hzellerController : public RGBMatrixController {
private:
  bool isDisplayActive;
  std::string filepath;

private:
  void RGB_Thread(){
    std::cout << "Thread started: LED-Matrix-Handling (infinite Loop)" << std::endl;
    while(true){
      if(this->isDisplayActive == true){
        std::cout << "Thread erkennt isDisplayActive: " << this->isDisplayActive << std::endl;
        bild_anzeigen(this->filepath);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      //std::cout << "Thread loops one more time" << std::endl;
    }
  }

public:
    hzellerController(){
        std::cout << "Initializing hzellerController... " << std::endl;
        this->isDisplayActive = false;
        this->filepath = "";

        std::thread LED_Matrix_Thread(&RGBMatrixController::RGB_Thread, this);
        LED_Matrix_Thread.detach();
        std::cout << "Done" << std::endl;
    }

    void bild_anzeigen(std::string s){ //shows Image/GIF

      //Anzahl Argumente der command line
      int terminal_argc = 2;

      //get length of input string
      int string_length = s.length();

      //declaring char array with "\0" in the end
      char imagepath[string_length + 1];

      //copying the contents of the string to char array
      std::strcpy(imagepath, s.c_str());

      //image_viewer_test6 -> Programmname -> argv[0]
      char program_name[] = "RGB_Matrix_Controller";

      //"../img/coordinates.png" -> Filepath -> argv[1]
      //char bildname[] = {'.','.','/','i','m','g','/','c','o','o','r','d','i','n','a','t','e','s','.','p','n','g','\0'};

      char *terminal_argv[] = {program_name, imagepath};

      Magick::InitializeMagick(*terminal_argv);

      //muss bei jedem Aufruf der bild_anzeigen() Funktion auf false gesetzt werden, da die Matrix sonst nichts anzeigt
      interrupt_received = false;

      RGBMatrix::Options matrix_options;
      rgb_matrix::RuntimeOptions runtime_opt;

      matrix_options.chain_length = 12; //12 LED panels
      matrix_options.cols = 32; //resolution of panels
      matrix_options.rows = 32; //resolution of panels
      matrix_options.parallel = 2; //two parallel chains
      matrix_options.pwm_bits = 11; //choose between 1 and 11 -> 2 is the best for this HW
      matrix_options.multiplexing = 2; //there are several multiplexing modes -> see documentation
      matrix_options.pixel_mapper_config = "U-mapper;Rotate:180"; //pixel mapper configuration
      matrix_options.show_refresh_rate = true; //not necessarily needed but may be interesting
      matrix_options.row_address_type = 3; //Default row address type is 0, corresponding to direct setting of the
                                            // row, while row address type 1 is used for panels that only have A/B,
                                            // typically some 64x64 panels

      runtime_opt.drop_privileges = 0; // If you drop privileges, the root user you start the program with
                                          // to be able to initialize the hardware will be switched to an unprivileged
                                          // user to minimize a potential security attack surface.
      runtime_opt.gpio_slowdown = 3; //2 is recommended for RPI4... but 4 works better for this matrix

      bool do_forever = false;
      bool do_center = false;
      bool do_shuffle = false;

      // We remember ImageParams for each image, which will change whenever
      // there is a flag modifying them. This map keeps track of filenames
      // and their image params (also for unrelated elements of terminal_argv[], but doesn't
      // matter).
      // We map the pointer instad of the string of the terminal_argv parameter so that
      // we can have two times the same image on the commandline list with different
      // parameters.
      std::map<const void *, struct ImageParams> filename_params;

      // Set defaults.
      ImageParams img_param;
      for (int i = 0; i < terminal_argc; ++i) {
        filename_params[terminal_argv[i]] = img_param;
      }

      const char *stream_output = NULL;

      int opt;
      while ((opt = getopt(terminal_argc, terminal_argv, "w:t:l:fr:c:P:LhCR:sO:V:D:")) != -1) {
        switch (opt) {
        case 'w':
          img_param.wait_ms = roundf(atof(optarg) * 1000.0f);
          break;
        case 't':
          img_param.anim_duration_ms = roundf(atof(optarg) * 1000.0f);
          break;
        case 'l':
          img_param.loops = atoi(optarg);
          break;
        case 'D':
          img_param.anim_delay_ms = atoi(optarg);
          break;
        case 'f':
          do_forever = true;
          break;
        case 'C':
          do_center = true;
          break;
        case 's':
          do_shuffle = true;
          break;
        case 'r':
          fprintf(stderr, "Instead of deprecated -r, use --led-rows=%s instead.\n",
                  optarg);
          matrix_options.rows = atoi(optarg);
          break;
        case 'c':
          fprintf(stderr, "Instead of deprecated -c, use --led-chain=%s instead.\n",
                  optarg);
          matrix_options.chain_length = atoi(optarg);
          break;
        case 'P':
          matrix_options.parallel = atoi(optarg);
          break;
        case 'L':
          fprintf(stderr, "-L is deprecated. Use\n\t--led-pixel-mapper=\"U-mapper\" --led-chain=4\ninstead.\n");
          return;
          break;
        case 'R':
          fprintf(stderr, "-R is deprecated. "
                  "Use --led-pixel-mapper=\"Rotate:%s\" instead.\n", optarg);
          return;
          break;
        case 'O':
          stream_output = strdup(optarg);
          break;
        case 'V':
          img_param.vsync_multiple = atoi(optarg);
          if (img_param.vsync_multiple < 1) img_param.vsync_multiple = 1;
          break;
        case 'h':
        default:
          return;
        }

        // Starting from the current file, set all the remaining files to
        // the latest change.
        for (int i = 1; i < terminal_argc; ++i) {
          filename_params[terminal_argv[i]] = img_param;
        }
      }

      const int filename_count = terminal_argc - 1;
      if (filename_count == 0) {
        fprintf(stderr, "Expected image filename.\n");
        return;
      }

      // Prepare matrix
      runtime_opt.do_gpio_init = (stream_output == NULL);
      RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
      if (matrix == NULL)
        return;

      FrameCanvas *offscreen_canvas = matrix->CreateFrameCanvas();

      printf("Size: %dx%d. Hardware gpio mapping: %s\n",
             matrix->width(), matrix->height(), matrix_options.hardware_mapping);

      // These parameters are needed once we do scrolling.
      const bool fill_width = false;
      const bool fill_height = false;

      // In case the output to stream is requested, set up the stream object.
      rgb_matrix::StreamIO *stream_io = NULL;
      rgb_matrix::StreamWriter *global_stream_writer = NULL;
      if (stream_output) {
        int fd = open(stream_output, O_CREAT|O_WRONLY, 0644);
        if (fd < 0) {
          perror("Couldn't open output stream");
          return;
        }
        stream_io = new rgb_matrix::FileStreamIO(fd);
        global_stream_writer = new rgb_matrix::StreamWriter(stream_io);
      }

      const tmillis_t start_load = GetTimeInMillis();
      fprintf(stderr, "Loading %d files...\n", terminal_argc - 1);
      // Preparing all the images beforehand as the Pi might be too slow to
      // be quickly switching between these. So preprocess.
      std::vector<FileInfo*> file_imgs;
      for (int imgarg = 1; imgarg < terminal_argc; ++imgarg) {
        const char *filename = terminal_argv[imgarg];
        FileInfo *file_info = NULL;

        std::string err_msg;
        std::vector<Magick::Image> image_sequence;
        if (LoadImageAndScale(filename, matrix->width(), matrix->height(),
                              fill_width, fill_height, &image_sequence, &err_msg)) {
          file_info = new FileInfo();
          file_info->params = filename_params[filename];
          file_info->content_stream = new rgb_matrix::MemStreamIO();
          file_info->is_multi_frame = image_sequence.size() > 1;
          rgb_matrix::StreamWriter out(file_info->content_stream);
          for (size_t i = 0; i < image_sequence.size(); ++i) {
            const Magick::Image &img = image_sequence[i];
            int64_t delay_time_us;
            if (file_info->is_multi_frame) {
              delay_time_us = img.animationDelay() * 10000; // unit in 1/100s
            } else {
              delay_time_us = file_info->params.wait_ms * 1000;  // single image.
            }
            if (delay_time_us <= 0) delay_time_us = 100 * 1000;  // 1/10sec
            StoreInStream(img, delay_time_us, do_center, offscreen_canvas,
                          global_stream_writer ? global_stream_writer : &out);
          }
        } else {
          // Ok, not an image. Let's see if it is one of our streams. -> Here it is actually written into a stream
          int fd = open(filename, O_RDONLY);
          if (fd >= 0) {
            file_info = new FileInfo();
            file_info->params = filename_params[filename];
            file_info->content_stream = new rgb_matrix::FileStreamIO(fd);
            StreamReader reader(file_info->content_stream);
            if (reader.GetNext(offscreen_canvas, NULL)) {  // header+size ok
              file_info->is_multi_frame = reader.GetNext(offscreen_canvas, NULL);
              reader.Rewind();
              if (global_stream_writer) {
                CopyStream(&reader, global_stream_writer, offscreen_canvas);
              }
            } else {
              err_msg = "Can't read as image or compatible stream";
              delete file_info->content_stream;
              delete file_info;
              file_info = NULL;
            }
          }
          else {
            perror("Opening file");
          }
        }

        if (file_info) {
          file_imgs.push_back(file_info);
        } else {
          fprintf(stderr, "%s skipped: Unable to open (%s)\n",
                  filename, err_msg.c_str());
        }
      }

      if (stream_output) { //this only prints out on the console
        delete global_stream_writer;
        delete stream_io;
        if (file_imgs.size()) {
          fprintf(stderr, "Done: Output to stream %s; "
                  "this can now be opened with led-image-viewer with the exact same panel configuration settings such as rows, chain, parallel and hardware-mapping\n", stream_output);
        }
        if (do_shuffle)
          fprintf(stderr, "Note: -s (shuffle) does not have an effect when generating streams.\n");
        if (do_forever)
          fprintf(stderr, "Note: -f (forever) does not have an effect when generating streams.\n");
        // Done, no actual output to matrix.
        return;
      }

      // Some parameter sanity adjustments.
      if (file_imgs.empty()) {
        // e.g. if all files could not be interpreted as image.
        fprintf(stderr, "No image could be loaded.\n");
        return;
      } else if (file_imgs.size() == 1) {
        // Single image: show forever.
        file_imgs[0]->params.wait_ms = distant_future;
      } else {
        for (size_t i = 0; i < file_imgs.size(); ++i) {
          ImageParams &params = file_imgs[i]->params;
          // Forever animation ? Set to loop only once, otherwise that animation
          // would just run forever, stopping all the images after it.
          if (params.loops < 0 && params.anim_duration_ms == distant_future) {
            params.loops = 1;
          }
        }
      }

      fprintf(stderr, "Loading took %.3fs; now: Display.\n",
              (GetTimeInMillis() - start_load) / 1000.0);

      signal(SIGTERM, InterruptHandler);
      signal(SIGINT, InterruptHandler);

      do {
        if (do_shuffle) {
          std::random_shuffle(file_imgs.begin(), file_imgs.end());
        }
        for (size_t i = 0; i < file_imgs.size() && !interrupt_received; ++i) {
          DisplayAnimation(file_imgs[i], matrix, offscreen_canvas); //here it is actually displayed (image, GIF or stream)
        }
      } while (do_forever && !interrupt_received);

      if (interrupt_received) {
        fprintf(stderr, "Caught signal. Exiting.\n");
      }

      // Animation finished. Shut down the RGB matrix.
      matrix->Clear();
      delete matrix;
    }

    void setIsDisplayActive(bool value){
      this->isDisplayActive = value;
    }

    void setFilepath(std::string value){
      this->filepath = value;
    }
};

//____________________________________________________________________________________________________________________________//


// Dummy RGB-Controller for use when testing the software on non-Raspberry Pi hardware.
class DummyRGBController : public RGBMatrixController {
private:
  bool isDisplayActive;
  std::string filepath;

private:
  void RGB_Thread(){
    std::cout << "Dummy_Thread started: LED-Matrix-Handling (infinite Loop)" << std::endl;
    while(true){
      if(this->isDisplayActive == true){
        std::cout << "Dummy_Thread erkennt isDisplayActive: " << this->isDisplayActive << std::endl;
        bild_anzeigen(this->filepath);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  }

public:
    DummyRGBController(){
        std::cout << "Initializing DummyRGBController... " << std::endl;
        this->isDisplayActive = false;
        this->filepath = "";

        //Start Threads
        std::thread LED_Matrix_Thread(&RGBMatrixController::RGB_Thread, this);
        LED_Matrix_Thread.detach();

        std::cout << "Done" << std::endl;
    }

  void bild_anzeigen(std::string s){

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    std::cout << "Showing animation: " << std::endl;
    std::cout << this->filepath << "\n" << std::endl;

    while(!interrupt_received){
      std::cout << ".";
      std::cout.flush();
      usleep(1000000);
    }

    if(interrupt_received) {
      fprintf(stderr, "Caught signal. Exiting.\n");
    }
  }

  void setIsDisplayActive(bool value){
    this->isDisplayActive = value;
  }

  void setFilepath(std::string value){
    this->filepath = value;
  }
};

#endif
