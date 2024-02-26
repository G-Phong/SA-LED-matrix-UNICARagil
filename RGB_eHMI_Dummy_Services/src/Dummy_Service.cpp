#include <asoa/core/runtime.hpp>
#include <asoa/core/task.hpp>

// TODO: Include Service
#include <Dummy_Service.hpp>

int main(){
    /* Initialisierung des RTPS Kommunikations Treibers */
    rtps_init();

    /* Definieren des Namens der Hardware auf der die Runtime läuft (Dieser Name wird später zur Identifikation der Runtime verwendet) */
    // TODO: Name anpassen
    char hardware_name[] = "DummyHW"; //freier Name der HW -> begrenzte String-Länge
    Runtime::init(hardware_name);

    /* Instanziieren des Services */
    // TODO: Name anpassen
    const char name[] = "Dummy_Serv"; //Service Name festlegen
    DummyService* service = new DummyService(name); //Selber Name wie Klasse in RGBService.hpp

    /* Uebergeben des Services and die Runtime */
    Runtime::get()->publishService(service);
    std::cout << "Looping" << std::endl;

    while (1) {
        /* Hauptschleife der Runtime starten */
        Runtime::get()->loop();
        std::cout << ".";
        std::cout.flush();
        usleep(1000000);
    }
}
