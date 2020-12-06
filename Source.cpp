#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>


const long long BUFFER_SIZE = 1000000;

std::vector<long long> PrefixFunction(std::vector<unsigned long long>& pattern, long long& pattern_size) {
	std::vector<long long> pi(pattern_size, 0);
	long long j;
	for (long long i = 1; i < pattern_size; ++i) {
		j = pi[i - 1];
		while ((j > 0) && (pattern[i] != pattern[j]))
			j = pi[j - 1];
		if (pattern[i] == pattern[j])
			++j;
		pi[i] = j;
	}
	return pi;
}

void KMP(std::vector<long long>& prefix, std::vector<unsigned long long>& pattern, std::vector<unsigned long long>& text, long long begin, std::vector<long long>& result) {
	for (long long k = 0, i = begin; i < text.size(); ++i) {
		while ((k > 0) && (pattern[k] != text[i])) {
			k = prefix[k - 1];
		}
		if (pattern[k] == text[i]) {
			++k;
		}
		if (k == pattern.size()) {
			k = prefix[k - 1];
			result.push_back(i - pattern.size() + 1);
		}
	}
}

void PrintResults(std::vector<long long>& result, std::vector<long long>& lines, long long& lines_count, long long pattern_size, long long& start_off) {
	long long prev_line = 0;
	for (long long i = 0, j = 0, k = 0; i < BUFFER_SIZE; ++i) {
		if (j < lines.size()) {
			if (i == lines[j]) {
				while (i == lines[j]) {
					++lines_count;
					++j;
					if (j >= lines.size())
						break;
				}
				--j;
				prev_line = lines[j];
				++j;
				start_off = 0;
				pattern_size = 0;
			}
		}
		if (k < result.size()) {
			if (i == result[k]) {
				long long b = result[k] - prev_line + start_off - pattern_size;
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

	std::vector<unsigned long long> pattern;

	std::vector<unsigned long long> buffer;
	buffer.reserve(BUFFER_SIZE);
	std::vector<unsigned long long> tmp;

	unsigned long long word;

	std::vector<long long> result, lines;
	long long unused_space = 0;

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
	long long pattern_size = (long long)pattern.size();
	std::vector<long long> prefix = PrefixFunction(pattern, pattern_size);

	bool first_search = true;
	long long lines_count = 0, prev = 0;

	while (std::cin.peek() == ' ') {
		std::cin.get();
	}
	while (std::cin.peek() == '\n') {
		std::cin.get();
		lines.push_back(buffer.size());
	}

	long long begin;
	while (std::cin >> word) {
		buffer.push_back(word);
		++unused_space;
		if (unused_space > BUFFER_SIZE - pattern.size()) {
			tmp.push_back(word);
		}

		if (buffer.size() == BUFFER_SIZE) {
			if (first_search) {
				begin = 0;
				pattern_size = 0;
				first_search = false;
			}
			else {
				begin = 1;
				pattern_size = (long long)pattern.size();
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
			lines.push_back(buffer.size());
		}
	}

	if (first_search) {
		begin = 0;
		pattern_size = 0;
	}
	else {
		begin = 0;
		pattern_size = (long long)pattern.size();
	}
	KMP(prefix, pattern, buffer, begin, result);
	PrintResults(result, lines, lines_count, pattern_size, prev);

	return 0;
}