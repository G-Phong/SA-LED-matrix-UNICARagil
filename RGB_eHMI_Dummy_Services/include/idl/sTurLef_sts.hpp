#include <asoa/core/functionality.hpp>

/*
 * Generated with the ASOA Functionality Generator.
 * Name: sTurLef_sts
 * ID: 10
 */

class sTurLef_sts: public RPCFunctionality
{
  private:
    std::string type_name_ = "sTurLef_sts";
    uint64_t type_id_ = 10;

  public:
    
    struct Parameter : public FuncComponentBase
    {
 
		bool set_turning_left_status;
 
		bool Parameter_serialize_topic(ucdrBuffer* writer, const Parameter* topic)
		{
		    (void) ucdr_serialize_bool(writer, topic->set_turning_left_status);
		
		    return !writer->error;
		}
		
		bool Parameter_deserialize_topic(ucdrBuffer* reader, Parameter* topic)
		{
		    (void) ucdr_deserialize_bool(reader, &topic->set_turning_left_status);
		
		    return !reader->error;
		}
		
		uint32_t Parameter_size_of_topic(const Parameter* topic, uint32_t size)
		{
		    uint32_t previousSize = size;
		    size += ucdr_alignment(size, 1) + 1;
		
		    return size - previousSize;
		}
 
        uint32_t maxSize() override
        {
             
            return Parameter_size_of_topic(this, 0);
             
        }

        bool deserialize(const uint8_t *buffer, uint32_t length) override
        {
             
            ucdrBuffer ucdr_buffer;
            ucdr_init_buffer(&ucdr_buffer, buffer, length);
            return Parameter_deserialize_topic(&ucdr_buffer, this);
             
        }

        long serialize(uint8_t *buffer, uint32_t max_size) override
        {
             
            ucdrBuffer ucdr_buffer;
            ucdr_init_buffer(&ucdr_buffer, buffer, max_size);
            bool ret = Parameter_serialize_topic(&ucdr_buffer, this);

            if(!ret){
                return -1;
            }else{
                return ucdr_buffer_length(&ucdr_buffer);
            }
             
        }
    };
 
    struct Return : public FuncComponentBase
    {
 
		bool success;
 
		bool Return_serialize_topic(ucdrBuffer* writer, const Return* topic)
		{
		    (void) ucdr_serialize_bool(writer, topic->success);
		
		    return !writer->error;
		}
		
		bool Return_deserialize_topic(ucdrBuffer* reader, Return* topic)
		{
		    (void) ucdr_deserialize_bool(reader, &topic->success);
		
		    return !reader->error;
		}
		
		uint32_t Return_size_of_topic(const Return* topic, uint32_t size)
		{
		    uint32_t previousSize = size;
		    size += ucdr_alignment(size, 1) + 1;
		
		    return size - previousSize;
		}
 

        bool deserialize(const uint8_t *buffer, uint32_t length) override
        {
             
            ucdrBuffer ucdr_buffer;
            ucdr_init_buffer(&ucdr_buffer, buffer, length);
            return Return_deserialize_topic(&ucdr_buffer, this);
             
        }

        long serialize(uint8_t *buffer, uint32_t max_size) override
        {
             
            ucdrBuffer ucdr_buffer;
            ucdr_init_buffer(&ucdr_buffer, buffer, max_size);
            bool ret = Return_serialize_topic(&ucdr_buffer, this);
            if (!ret)
            {
                return -1;
            }
            else
            {
                return ucdr_buffer_length(&ucdr_buffer);
            }
             
        }

        virtual uint32_t maxSize() override
        {
             
            return Return_size_of_topic(this, 0);
             
        }

    };

    void RPCFunctionality()
    {
    }

    const char* getTypeName() override
    {
        return type_name_.c_str();
    }

    std::uint32_t getTypeID() override
    {
        return type_id_;
    }
};