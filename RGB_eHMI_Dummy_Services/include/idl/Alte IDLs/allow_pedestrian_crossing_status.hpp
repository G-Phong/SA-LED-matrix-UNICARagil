#include <asoa/core/functionality.hpp>

/*
 * Generated with the ASOA Functionality Generator.
 * Name: allow_pedestrian_crossing_status
 * ID: 10
 */

class allow_pedestrian_crossing_status: public Functionality
{
  private:
    std::string type_name_ = "allow_pedestrian_crossing_status";
    uint64_t type_id_ = 10;

  public:
    
    struct Data : public FuncComponentBase
    {
 
		bool allow_pedestrian_crossing;
		float time_distance_pedestrian;
		float distance_pedestrian;
 
		bool Data_serialize_topic(ucdrBuffer* writer, const Data* topic)
		{
		    (void) ucdr_serialize_bool(writer, topic->allow_pedestrian_crossing);
		
		    (void) ucdr_serialize_float(writer, topic->time_distance_pedestrian);
		
		    (void) ucdr_serialize_float(writer, topic->distance_pedestrian);
		
		    return !writer->error;
		}
		
		bool Data_deserialize_topic(ucdrBuffer* reader, Data* topic)
		{
		    (void) ucdr_deserialize_bool(reader, &topic->allow_pedestrian_crossing);
		
		    (void) ucdr_deserialize_float(reader, &topic->time_distance_pedestrian);
		
		    (void) ucdr_deserialize_float(reader, &topic->distance_pedestrian);
		
		    return !reader->error;
		}
		
		uint32_t Data_size_of_topic(const Data* topic, uint32_t size)
		{
		    uint32_t previousSize = size;
		    size += ucdr_alignment(size, 1) + 1;
		
		    size += ucdr_alignment(size, 4) + 4;
		
		    size += ucdr_alignment(size, 4) + 4;
		
		    return size - previousSize;
		}
 
        uint32_t maxSize() override
        {
             
            return Data_size_of_topic(this, 0);
             
        }

        bool deserialize(const uint8_t *buffer, uint32_t length) override
        {
             
            ucdrBuffer ucdr_buffer;
            ucdr_init_buffer(&ucdr_buffer, buffer, length);
            return Data_deserialize_topic(&ucdr_buffer, this);
             
        }

        long serialize(uint8_t *buffer, uint32_t max_size) override
        {
             
            ucdrBuffer ucdr_buffer;
            ucdr_init_buffer(&ucdr_buffer, buffer, max_size);
            bool ret = Data_serialize_topic(&ucdr_buffer, this);

            if(!ret){
                return -1;
            }else{
                return ucdr_buffer_length(&ucdr_buffer);
            }
             
        }
    } data_;
 
    struct Quality : public FuncComponentBase
    {
 
		
 

        bool deserialize(const uint8_t *buffer, uint32_t length) override
        {
             
        }

        long serialize(uint8_t *buffer, uint32_t max_size) override
        {
            return 0; 
        }

        virtual uint32_t maxSize() override
        {
            return 0; 
        }

    } quality_;
 
    struct Parameter : public FuncComponentBase
    {
 
		
 
        bool deserialize(const uint8_t *buffer, uint32_t length) override
        {
             
        }

        long serialize(uint8_t *buffer, uint32_t max_size) override
        {
            return 0; 
        }

        virtual uint32_t maxSize() override
        {
            return 0; 
        }

    } parameter_;

    void Functionality()
    {
    }

    const std::string &getTypeName() override
    {
        return type_name_;
    }

    std::uint32_t getTypeID() override
    {
        return type_id_;
    }
};