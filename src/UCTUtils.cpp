#include "UCTUtils.h"
#include "UCTSave.h"

namespace uct {
namespace utils {

std::string format(std::string formatString, const std::vector<std::string>& args) {
	for (std::size_t i = 0; i < args.size(); i++) {
		std::size_t formatPos = formatString.find_first_of("%") + 2;
		std::string start = formatString.substr(0, formatPos);
		std::string end = formatString.substr(formatPos, formatString.size() - formatPos);
		start = format(start, args[i]);
		formatString = start + end;
	}
	return formatString;
}

std::string format(const save::Template& t) {
	std::string formatString = t.templateRaw;
	for (std::size_t i = 0; i < t.templateSlots.size(); i++) {
		std::size_t formatPos = formatString.find_first_of("%") + 2;
		std::string start = formatString.substr(0, formatPos);
		std::string end = formatString.substr(formatPos, formatString.size() - formatPos);
		start = format(start, t.templateSlots[i].valueBuffer);
		formatString = start + end;
	}
	return formatString;
}

} // namespace utils
} // namespace uct
