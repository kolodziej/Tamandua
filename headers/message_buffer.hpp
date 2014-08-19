#ifndef TAMANDUA_MESSAGE_BUFFER_HPP
#define TAMANDUA_MESSAGE_BUFFER_HPP
#include <memory>
#include <string>
#include "message_header.hpp"


namespace tamandua
{
	class message_buffer
	{
		private:
			std::shared_ptr<char> buffer_;
			size_t buffer_size_;

		public:
			message_buffer(std::shared_ptr<char> buffer, size_t buffer_size) : buffer_(buffer), buffer_size_(buffer_size) {};
			message_buffer(message_header, std::string);

			std::shared_ptr<char> get_buffer();
			size_t get_buffer_size();
			message_header get_message_header();
			std::string get_message();

		private:
			void compose_(message_header, std::string);
	};
}

#endif
