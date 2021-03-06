#ifndef __ANTIVIR_DYNAMIC_DETECTION_H__
#define __ANTIVIR_DYNAMIC_DETECTION_H__

#include <map>

#include "descriptortracer.h"
#include "tracer.h"

namespace antivirus
{
	/**
	* Performs a dynamic detectition to see if the process had a malicious behaviour
	*/
	class DynamicDetection
	{
		public:
			typedef DescriptorTracer::filename filename;
			typedef Tracer::syscall_name	   syscall_name;

			static DynamicDetection* get_instance();
			virtual ~DynamicDetection();

			void initialize();

			inline bool is_malicious() { return _is_malicious; }

		private:
			std::map<syscall_name, std::map<filename, std::string> > _malicious_actions;
			std::map<pid_t, std::map<filename, std::string> > _written_content;
			std::map<filename, int> _base_offset;
			std::map<filename, int> _content_offset;

			bool _is_malicious;

			DynamicDetection();

			bool _check_if_action_is_malicious(pid_t pid, syscall_name syscall, filename file);
			void _stash_new_output(pid_t pid, filename file, const std::string& output);

			void _change_pointer_position(filename file, int new_offset);

			bool _does_it_match(const std::string& str1, const std::string& str2) const;

			static size_t _custom_read_string_at(long addr, char* buffer, size_t len, pid_t pid);

			static bool _write_handler(pid_t pid, struct user_regs_struct& regs);
			static bool _lseek_handler(pid_t pid, struct user_regs_struct& regs);
	};
}

#endif
