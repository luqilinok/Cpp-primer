#include <memory>
#include <iostream>
#include <algorithm>

class String
{
	friend std::ostream& operator<<(std::ostream&, const String&);
	friend std::istream& operator>>(std::istream&, String&);
	friend bool operator==(const String&, const String&);
	friend bool operator!=(const String&, const String&);
	friend bool operator< (const String&, const String&);
	friend bool operator> (const String&, const String&);
	friend bool operator<=(const String&, const String&);
	friend bool operator>=(const String&, const String&);

public:
	String() : String("") { }
	String(const char *);
	String(const String&);
	String& operator=(const String&);
	String(String &&) noexcept;
	String& operator=(String&&)noexcept;
	~String();

	void push_back(const char);

	char* begin() const { return elements; }
	char* end() const { return last_elem; }

	const char *c_str() const { return elements; }
	size_t size() const { return last_elem - elements; }
	size_t length() const { return size(); }
	size_t capacity() const { return cap - elements; }

	void reserve(size_t);
	void resize(size_t);
	void resize(size_t, char);

private:
	std::pair<char*, char*> alloc_n_copy(const char*, const char*);
	void range_initializer(const char*, const char*);
	void free();
	void reallocate();
	void alloc_n_move(size_t new_cap);
	void chk_n_alloc() { if (first_free == cap) reallocate(); }

private:
	char *elements;
	char *last_elem;
	char *first_free;
	char *cap;
	std::allocator<char> alloc;
};

std::ostream& operator<<(std::ostream&, const String&);
std::istream& operator>>(std::istream&, String&);
bool operator==(const String&, const String&);
bool operator!=(const String&, const String&);
bool operator< (const String&, const String&);
bool operator> (const String&, const String&);
bool operator<=(const String&, const String&);
bool operator>=(const String&, const String&);

std::ostream& operator<<(std::ostream &os, const String &lhs)
{
	os << lhs.c_str();
	return os;
}

std::istream& operator>>(std::istream &is, String &rhs)
{
	for (char c; (c = is.get()) != '\n';) {
		rhs.push_back(c);
	}
	return is;
}

bool operator==(const String &lhs, const String &rhs)
{
	return (lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

bool operator!=(const String &lhs, const String &rhs)
{
	return !(lhs == rhs);
}

bool operator<(const String &lhs, const String &rhs)
{
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

bool operator>(const String &lhs, const String &rhs)
{
	return rhs < lhs;
}

bool operator<=(const String &lhs, const String &rhs)
{
	return !(rhs < lhs);
}

bool operator>=(const String &lhs, const String &rhs)
{
	return !(lhs < rhs);
}

String::String(const char *s)
{
	char *sl = const_cast<char*>(s);
	while (*sl)
		++sl;
	range_initializer(s, ++sl);
}

String::String(const String& rhs)
{
	range_initializer(rhs.elements, rhs.first_free);
}

String& String::operator = (const String &rhs)
{
	auto newstr = alloc_n_copy(rhs.elements, rhs.first_free);
	free();
	elements = newstr.first;
	first_free = cap = newstr.second;
	last_elem = first_free - 1;
	return *this;
}

String::String(String &&s) noexcept : elements(s.elements), last_elem(s.last_elem), first_free(s.first_free), cap(s.cap)
{
	s.elements = s.last_elem = s.first_free = s.cap = nullptr;
}

String& String::operator = (String &&rhs) noexcept
{
	if (this != &rhs) {
		free();
		elements = rhs.elements;
		last_elem = rhs.last_elem;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.last_elem = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}

String::~String()
{
	free();
}

void String::push_back(const char c)
{
	chk_n_alloc();
	*last_elem = c;
	last_elem = first_free;
	alloc.construct(first_free++, '\0');
}

void String::reallocate()
{
	auto newcapacity = size() ? 2 * (size() + 1) : 2;
	alloc_n_move(newcapacity);
}

void String::alloc_n_move(size_t new_cap)
{
	auto newdata = alloc.allocate(new_cap);
	auto dest = newdata;
	auto elem = elements;
	for (size_t i = 0; i != size() + 1; ++i)
		alloc.construct(dest++, std::move(*elem++));
	free();
	elements = newdata;
	last_elem = dest - 1;
	first_free = dest;
	cap = elements + new_cap;
}

void String::free()
{
	if (elements) {
		std::for_each(elements, first_free, [this](char &c) { alloc.destroy(&c); });
		alloc.deallocate(elements, cap - elements);
	}
}

std::pair<char*, char*>
String::alloc_n_copy(const char *b, const char *e)
{
	auto str = alloc.allocate(e - b);
	return{ str, std::uninitialized_copy(b, e, str) };
}

void String::range_initializer(const char *first, const char *last)
{
	auto newstr = alloc_n_copy(first, last);
	elements = newstr.first;
	first_free = cap = newstr.second;
	last_elem = first_free - 1;
}

void String::reserve(size_t new_cap)
{
	if (new_cap <= capacity()) return;
	alloc_n_move(new_cap);
}

void String::resize(size_t count, char c)
{
	if (count > size()) {
		if (count > capacity()) reserve(count * 2);
		for (size_t i = size(); i != count; ++i) {
			*last_elem++ = c;
			alloc.construct(first_free++, '\0');
		}
	}
	else if (count < size()) {
		while (last_elem != elements + count) {
			--last_elem;
			alloc.destroy(--first_free);
		}
		*last_elem = '\0';
	}
}

void String::resize(size_t count)
{
	resize(count, ' ');
}
