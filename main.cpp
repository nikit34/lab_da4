#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>


struct elem_type {
	std::string str;
	uint64_t line;
	uint64_t line_pos;
};

class DTVector {
public:
	DTVector(size_t new_capacity) {
		//storage.reserve(new_capacity);
		storage = std::unique_ptr<elem_type[]>(new elem_type[new_capacity]);
		capacity = new_capacity;
		size = 0;
		first_elem = 0;
	}
	bool Empty() {
		return this->size == 0;
	}
	size_t Size() {
		return this->size;
	}
	elem_type& operator[](size_t index) {
		return this->storage[(this->first_elem + index) % this->capacity];
	}
	void PushBack(elem_type elem) {
		if (this->size == this->capacity)
			throw std::length_error("Size nobody has not equal capacity");
		this->storage[(this->first_elem + this->size) % this->capacity] = elem;
		++this->size;
	}
	void RemoveFirst(size_t num) {
		if (num > this->size)
			num = this->size;
		this->first_elem = (this->first_elem + num) % this->capacity;
		this->size = this->size - num;
	}
private:
	std::unique_ptr<elem_type[]> storage;
	//std::vector<elem_type> storage;
	size_t first_elem, size, capacity;
};


int main() {
	std::vector<std::string> pattern;

	std::string first_line;
	std::getline(std::cin, first_line);

	std::string tmp;
	std::istringstream iss(first_line);

	while (iss >> tmp) {
		for (uint64_t i = 0; i < tmp.size(); ++i) {
			char c = tmp[i];
			if (c >= 'A' && c <= 'Z')
				c = c - 'A' + 'a';
			tmp[i] = c;
		}
		pattern.push_back(tmp);
	}

	uint64_t n = pattern.size();
	std::vector<uint64_t> z(n, 0);

	for (uint64_t k = 1, l = 0, r = 0; k < n; ++k) {
		if (k <= r)
			z[k] = std::min(r - k + 1, z[k - l]);
		while (k + z[k] < n && pattern[z[k]] == pattern[k + z[k]])
			z[k]++;
		if (k + z[k] - 1 > r) {
			l = k;
			r = k + z[k] - 1;
		}
	}

	std::vector<uint64_t> sp(n, 0);
	for (uint64_t j = n - 1; j >= 1; --j) {
		uint64_t i = j + z[j] - 1;
		sp[i] = z[j];
	}


	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	char c = 'a';
	DTVector text(pattern.size());
	std::string temp_word;
	uint64_t line, pos_in_line;
	line = pos_in_line = 0;

	uint64_t pattern_index = 0;
	uint64_t text_index = 0;
	uint64_t pattern_size = pattern.size();

	while (c != EOF) {
		while (text.Size() < pattern_size) {
			c = std::getchar();
			if (c >= 'A' && c <= 'Z') {
				c = c - 'A' + 'a';
			}
			if (c == ' ' || c == '\n' || c == '\t' || c == EOF) {
				if (!temp_word.empty()) {

					elem_type temp_struct;
					temp_struct.str = temp_word;
					temp_struct.line = line;
					temp_struct.line_pos = pos_in_line;
					text.PushBack(temp_struct);

					temp_word.clear();
					pos_in_line++;
				}
				if (c == '\n') {
					line++;
					pos_in_line = 0;
				}
			}
			else {
				temp_word += c;
			}

			if (c == EOF)
				break;
		}
		if (text.Size() < pattern_size)
			break;

		while (pattern_index < pattern_size && pattern[pattern_index] == text[text_index].str) {
			pattern_index++;
			text_index++;
		}

		if (pattern_index == pattern_size) {
			std::cout << text[text_index - pattern_size].line + 1 << ", " << text[text_index - pattern_size].line_pos + 1 << std::endl;
		}

		if (pattern_index == 0) {
			pattern_index++;
			text_index = 0;
		}

		text.RemoveFirst(pattern_index - sp[pattern_index - 1]);
		pattern_index = sp[pattern_index - 1];
		text_index = pattern_index;
	}
}