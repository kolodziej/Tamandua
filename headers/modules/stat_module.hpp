#ifndef TAMANDUA_STAT_MODULE_HPP
#define TAMANDUA_STAT_MODULE_HPP
#include "../module_base.hpp"
#include "../server.hpp"
#include <map>

namespace tamandua
{
	class stat_module :
		public module_base
	{
		private:
			struct msg_stat
			{
				unsigned long long int num;
				unsigned long long int bytes;
				msg_stat() : num(0), bytes(0) {}
			};
			std::map<id_number_t, msg_stat> users_messages_num_;
			msg_stat general_, processed_, commands_;
			unsigned long long int participants_, groups_;

		public:
			stat_module(server &, command_interpreter &);

			void message_receive(std::shared_ptr<user>, message &);
			void message_processed(std::shared_ptr<user>, message &, processing_status);
			void new_participant(std::shared_ptr<participant>);
			void new_group(std::shared_ptr<group>);
			void participant_leave(std::shared_ptr<participant>);
			void group_close(std::shared_ptr<group>);

			void cmd_statistics(std::shared_ptr<user>, message&);
	};
}

#endif
