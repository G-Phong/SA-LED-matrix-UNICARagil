#include <asoa/core/functionality.hpp>

/*
 * Generated with the ASOA Functionality Generator.
 * Name: RPL_sts
 * ID: 10
 */

class RPL_sts: public Functionality
{
  private:
    std::string type_name_ = "RPL_sts";
    uint64_t type_id_ = 10;

  public:
    
    struct Data : public FuncComponentBase
    {
 
		bool rectangular_parking_left;
		bool parking_aborted;
		bool parking;
 
		bool Data_serialize_topic(ucdrBuffer* writer, const Data* topic)
		{
		    (void) ucdr_serialize_bool(writer, topic->rectangular_parking_left);
		
		    (void) ucdr_serialize_bool(writer, topic->parking_aborted);
		
		    (void) ucdr_serialize_bool(writer, topic->parking);
		
		    return !writer->error;
		}
		
		bool Data_deserialize_topic(ucdrBuffer* reader, Data* topic)
		{
		    (void) ucdr_deserialize_bool(reader, &topic->rectangular_parking_left);
		
		    (void) ucdr_deserialize_bool(reader, &topic->parking_aborted);
		
		    (void) ucdr_deserialize_bool(reader, &topic->parking);
		
		    return !reader->error;
		}
		
		uint32_t Data_size_of_topic(const Data* topic, uint32_t size)
		{
		    uint32_t previousSize = size;
		    size += ucdr_alignment(size, 1) + 1;
		
		    size += ucdr_alignment(size, 1) + 1;
		
		    size += ucdr_alignment(size, 1) + 1;
		
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