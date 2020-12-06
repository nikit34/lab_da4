#include<iostream>
#include<algorithm>
#include<string>
#include<cmath>


const uint64_t BUFFER_SIZE = 100000000;

template <typename DT>
class GVector {
private:
	DT* data;
	uint64_t capacity;
	uint64_t size;

public:
    GVector(uint64_t n=10000);
	GVector(uint64_t n, DT&& default_value);

	~GVector() {
		delete[] this->data;
		this->size = 0;
		this->capacity = 0;
		this->data = nullptr;
	}

	void PushBack(DT data);

	void Reserve(uint64_t size);

	uint64_t Size() const;
	DT& operator[](uint64_t index);
	DT operator[](uint64_t index) const;
};

template<typename DT>
GVector<DT>::GVector(uint64_t n): data(new DT[n]), capacity(n), size(0){ }

template<typename DT>
GVector<DT>::GVector(uint64_t n, DT&& default_value) {
	this->capacity = n;
	this->data = new DT[this->capacity];
	this->size = this->capacity;
	for (uint64_t i = 0; i < this->size; ++i)
		this->data[i] = default_value;
}

template <typename DT>
void GVector<DT>::PushBack(DT data) {
	if (this->size + 1 == this->capacity) {
		uint64_t newStorageSize = this->capacity ? this->capacity * 2 : 1;
		DT* newStorage = new DT[newStorageSize];
		std::copy(this->data, this->data + this->capacity, newStorage);
		delete[] this->data;
		this->data = newStorage;
		this->capacity = newStorageSize;
	}
	this->data[this->size] = data;
	this->size++;
}

template<typename DT>
void GVector<DT>::Reserve(uint64_t size){
	if (size <= this->capacity)
		return;

	DT* tmp = new DT[size];
	for (uint64_t i = 0; i < this->capacity; ++i)
		tmp[i] = this->data[i];

	delete[] this->data;
	this->data = tmp;
	this->capacity = size;
}

template <typename DT>
uint64_t GVector<DT>::Size() const {
	return this->size;
}

template <typename DT>
DT& GVector<DT>::operator[](uint64_t index) {
	if (index >= this->size) {
		std::cout << "Error: data index out of bounds";
		exit(0);
	}
	return *(this->data + index);
}

template <typename DT>
DT GVector<DT>::operator[](uint64_t index) const {
	if (index >= this->size) {
		std::cout << "Error: data index out of bounds";
		exit(0);
	}
	return *(this->data + index);
}

bool CaseCompare(std::string& s1, std::string& s2) {
	transform(s1.begin(), s1.end(), s1.begin(), tolower);
	transform(s2.begin(), s2.end(), s2.begin(), tolower);
	if (s1.compare(s2) == 0)
		return true;
	return false;
}

void PrefixFunction(GVector<std::string>& pattern, GVector<uint64_t>& pi) {
	uint64_t i, j;
	for (i = 1; i < pattern.Size(); ++i) {
		j = pi[i - 1];
		while ((j > 0) && !CaseCompare(pattern[i], pattern[j]))
			j = pi[j - 1];
		if (CaseCompare(pattern[i], pattern[j]))
			++j;
		pi[i] = j;
	}
}

void KMP(GVector<uint64_t>& prefix, GVector<std::string>& pattern, GVector<std::string>& text, uint16_t& begin, GVector<int64_t>& result) {
	int64_t i, k;
	uint64_t text_size = text.Size();
	uint64_t pattern_size = pattern.Size();
	for (k = 0, i = begin; i < text_size; ++i) {
		while ((k > 0) && !CaseCompare(pattern[k], text[i])) {
			k = prefix[k - 1];
		}
		if (CaseCompare(pattern[k], text[i])) {
			++k;
		}
		if (k == pattern_size) {
			k = prefix[k - 1];
			result.PushBack(i - pattern_size + 1);
		}
	}
}

void PrintResults(GVector<int64_t>& result, GVector<int64_t>& lines, int64_t& lines_count, uint64_t& pattern_size, int64_t& start_off) {
	int64_t prev_line = 0;
	int64_t i, j, k, b;
	uint64_t lines_size = lines.Size();
	uint64_t result_size = result.Size();
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

	GVector<std::string> pattern;

	GVector<std::string> buffer;
	buffer.Reserve(BUFFER_SIZE);
	GVector<std::string> tmp;

	std::string word;

	GVector<int64_t> result, lines;

	while (std::cin >> word) {
		pattern.PushBack(word);
		while (std::cin.peek() == ' ') {
			std::cin.get();
		}
		if (std::cin.peek() == '\n') {
			std::cin.get();
			break;
		}
	}

	uint64_t pattern_size = (uint64_t)pattern.Size();
	uint64_t used_space = (uint64_t)buffer.Size();

	GVector<uint64_t> prefix(pattern_size, 0);
	PrefixFunction(pattern, prefix);

	bool first_search = true;
	int64_t lines_count = 0, prev = 0;

	while (std::cin.peek() == ' ') {
		std::cin.get();
	}
	while (std::cin.peek() == '\n') {
		std::cin.get();
		lines.PushBack(used_space);
	}

	uint16_t begin;
	while (std::cin >> word) {
		buffer.PushBack(word);
		++used_space;
		if (used_space > BUFFER_SIZE - pattern_size) {
			tmp.PushBack(word);
		}

		if (used_space == BUFFER_SIZE) {
			if (first_search) {
				begin = 0;
				pattern_size = 0;
				first_search = false;
			}
			else {
				begin = 1;
				pattern_size = (uint64_t)pattern.Size();
			}
			KMP(prefix, pattern, buffer, begin, result);
			PrintResults(result, lines, lines_count, pattern_size, prev);

			buffer = tmp;
			used_space = buffer.Size();
		}
		while (std::cin.peek() == ' ') {
			std::cin.get();
		}
		while (std::cin.peek() == '\n') {
			std::cin.get();
			lines.PushBack(used_space);
		}
	}

	if (first_search) {
		begin = 0;
		pattern_size = 0;
	}
	else{
		begin = 1;
		pattern_size = (uint64_t)pattern.Size();
	}
	KMP(prefix, pattern, buffer, begin, result);
	PrintResults(result, lines, lines_count, pattern_size, prev);
	return 0;
}