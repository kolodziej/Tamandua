#ifndef TAMANDUA_HISTORY_MODULE_HPP
#define TAMANDUA_HISTORY_MODULE_HPP
#include "../module_base.hpp"
#include "../server.hpp"
#include "../hidden_participant.hpp"
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
			class history_user :
				public hidden_participant
			{
				private:
					const size_t history_size_;
					std::deque<const message&> history_;
					
				public:
					history_user(server &, id_number_t);
					void deliver_message(const message &);
			};

			std::vector<std::shared_ptr<history_user>> users_;
			std::thread history_thread;
			
		public:
			history_module(server &, command_interpreter &);

			void new_group(std::shared_ptr<group>);
			
	};
}

#endif
