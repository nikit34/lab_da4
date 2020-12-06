#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<cmath>


const uint32_t BUFFER_SIZE = 262145;

bool CaseCompare(std::string& s1, std::string& s2) {
	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	if (s1.compare(s2) == 0)
		return true;
	return false;
}

void PrefixFunction(std::vector<std::string>& pattern, std::vector<uint32_t>& pi) {
	uint32_t i, j;
	for (i = 1; i < pattern.size(); ++i) {
		j = pi[i - 1];
		while ((j > 0) && !CaseCompare(pattern[i], pattern[j]))
			j = pi[j - 1];
		if (CaseCompare(pattern[i], pattern[j]))
			++j;
		pi[i] = j;
	}
}

void KMP(std::vector<uint32_t>& prefix, std::vector<std::string>& pattern, std::vector<std::string>& text, uint32_t& begin, std::vector<int32_t>& result) {
	uint32_t i, k;
	uint32_t text_size = text.size();
	uint32_t pattern_size = pattern.size();
	for (k = 0, i = begin; i < text_size; ++i) {
		while ((k > 0) && !CaseCompare(pattern[k], text[i])) {
			k = prefix[k - 1];
		}
		if (CaseCompare(pattern[k], text[i])) {
			++k;
		}
		if (k == pattern_size) {
			k = prefix[k - 1];
			result.push_back(i - pattern_size + 1);
		}
	}
}

void PrintResults(std::vector<int32_t>& result, std::vector<int32_t>& lines, uint32_t& lines_count, uint32_t& pattern_size, uint32_t& start_off) {
	uint32_t prev_line = 0;
	int32_t i, j, k, b;
	uint32_t lines_size = lines.size();
	uint32_t result_size = result.size();
	for (i = 0, j = 0, k = 0; i < BUFFER_SIZE; ++i) {
		if (j < lines_size) {
			if (i == lines[j]) {
				while (i == lines[j]) {
					++lines_count;
					++j;
					if (j >= lines_size)
						break;
				}
				prev_line = lines[j - 1];
				start_off = 0;
				pattern_size = 0;
			}
		}
		if (k < result_size) {
			if (i == result[k]) {
				b = result[k] - prev_line + start_off - pattern_size;
				if (b < 0) {
					std::cout << lines_count << ", " << BUFFER_SIZE + b << std::endl;
				}
				else {
					std::cout << lines_count + 1 << ", " << b + 1 << std::endl;
				}
				++k;
			}
		}
	}
	start_off += BUFFER_SIZE - prev_line - pattern_size;
}


int main() {
	std::cin.sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::vector<std::string> pattern;

	std::vector<std::string> buffer;
	buffer.reserve(BUFFER_SIZE);
	std::vector<std::string> tmp;

	std::string word;

	std::vector<int32_t> result, lines;

	while (std::cin >> word) {
		pattern.push_back(word);
		while (std::cin.peek() == ' ') {
			std::cin.get();
		}
		if (std::cin.peek() == '\n') {
			std::cin.get();
			break;
		}
	}
	
	uint32_t pattern_size = pattern.size();
	uint32_t unused_space = (uint32_t)buffer.size();

	std::vector<uint32_t> prefix(pattern_size);
	PrefixFunction(pattern, prefix);

	bool first_search = true;
	uint32_t lines_count = 0, prev = 0;

	while (std::cin.peek() == ' ') {
		std::cin.get();
	}
	while (std::cin.peek() == '\n') {
		std::cin.get();
		lines.push_back(unused_space);
	}

	uint32_t begin;
	while (std::cin >> word) {
		buffer.push_back(word);
		++unused_space;
		if (unused_space > BUFFER_SIZE - pattern_size) {
			tmp.push_back(word);
		}

		if (unused_space == BUFFER_SIZE) {
			if (first_search) {
				begin = 0;
				pattern_size = 0;
				first_search = false;
			}
			else {
				begin = 1;
				pattern_size = (uint32_t)pattern.size();
			}
			KMP(prefix, pattern, buffer, begin, result);
			PrintResults(result, lines, lines_count, pattern_size, prev);

			buffer = tmp;
			unused_space = buffer.size();
			tmp.clear();
			lines.clear();
			result.clear();
		}
		while (std::cin.peek() == ' ') {
			std::cin.get();
		}
		while (std::cin.peek() == '\n') {
			std::cin.get();
			lines.push_back(unused_space);
		}
	}

	begin = 0;
	if (first_search) 
		pattern_size = 0;
	else 
		pattern_size = (uint32_t)pattern.size();
	KMP(prefix, pattern, buffer, begin, result);
	PrintResults(result, lines, lines_count, pattern_size, prev);

	return 0;
}