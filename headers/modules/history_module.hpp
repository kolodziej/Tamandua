#ifndef TAMANDUA_HISTORY_MODULE_HPP
#define TAMANDUA_HISTORY_MODULE_HPP
#include "../module_base.hpp"
#include "../server.hpp"
#include "../hidden_participant.hpp"
#include <map>
#include <deque>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace tamandua
{
	class history_module :
		public module_base
	{
		private:
			const size_t history_size_;
			class history_user :
				public hidden_participant
			{
				private:
					const size_t history_size_;
					std::deque<message> history_;
					
				public:
					history_user(server &, id_number_t, size_t hs);
					void deliver_message(const message &);
					const std::deque<message> & get_history();
					void read_message() {}
			};

			std::map<id_number_t, std::shared_ptr<history_user>> users_;
			
		public:
			history_module(server &, command_interpreter &, size_t);
			history_module(const history_module &) = delete;
	
			void cmd_history(std::shared_ptr<user>, message&);
			void new_group(std::shared_ptr<group>);
			const std::deque<message> & get_history(id_number_t);
	};
}

#endif
