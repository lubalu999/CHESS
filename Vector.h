#pragma once
#include <memory>


///Custom iterator
template<class T>
class vec_it;


///Custom Vector class

template<class T>
class my_vec {
private:
	const static int min_cap = 4;
	size_t capacity;
	size_t _size;
	T *data;
	/*std::allocator<T> allocator;*/
public:
	/// Initializing constructor
	my_vec() : _size(0), capacity(min_cap), data(new T[min_cap]()) {}

	/// A copy constructor
	my_vec(const my_vec<T> & a) {
		data = new T[a.capacity]();		//выделение памяти
		capacity = a.capacity;
		_size = a._size;
		for (size_t i = 0; i < _size; i++)
			data[i] = a.data[i];
	}

	/// Moving constructor
	my_vec(my_vec<T> && a) {
		data = a.data;
		_size = a._size;
		capacity = a.capacity;
		a.data = nullptr;
		a.capacity = 0;
	}

	~my_vec() {
		delete[] data;
	}

	size_t size() const { return _size; }

	bool empty() const { return !_size; }

	void resize(size_t new_capacity) {
		if (new_capacity != capacity) {
			capacity = new_capacity;
			T *buf = data;
			data = new T[new_capacity]();
			if (_size > new_capacity)
				_size = new_capacity;
			for (size_t i = 0; i < _size; i++)
				data[i] = std::move(buf[i]);
		}
	}

	/*!
		Adds an element to the end of the vector by copying
	*/
	void push_back(const T &val) {
		if (capacity == _size)
			resize(capacity + min_cap);
		data[_size] = val;
		_size++;
	}

	/*!
		Adds an element to the end of the vector by moving
	*/
	void push_back(T &&a) {
		if (capacity == _size)
			resize(capacity + min_cap);
		data[_size] = std::move(a);
		_size++;
	}

	/*!
		Overloaded copy assignment operator
	*/
	my_vec<T> &operator=(const my_vec<T> &a) {
		if (this == &a)
			return *this;
		resize(a.capacity);
		std::copy(a.begin(), a.end(), data);
		return *this;
	}

	/*!
		Overloaded assignment operator by moving
	*/
	my_vec<T> &operator=(my_vec<T> &&a) {
		if (this == &a)
			return *this;
		T *buf = data;
		int sbuf = _size, scap = capacity;
		data = a.data;
		capacity = a.capacity;
		_size = a._size;
		a.data = buf;
		a.capacity = scap;
		a._size = sbuf;
		return *this;
	}

	/*!
		Overloaded index access operator for reading
	*/
	const T &operator[](int i) const {
		if (i < 0 || i >= capacity)
			throw std::runtime_error("Wrong index");
		return data[i];
	}

	/*!
		Overloaded index access operator for reading and modifying
	*/
	T &operator[](int i) {
		if (i < 0 || i >= capacity)
			throw std::runtime_error("Wrong index");
		return data[i];
	}

	/*!
		Operator equals
	*/

	bool operator==(const my_vec<T>& other) const {
		if (_size != other._size)
			return false;
		for (size_t i = 0; i < _size; i++)
			if (data[i] != other.data[i])
				return false;
		return true;
	}

	/*!
		Operator not equals
	*/

	bool operator!=(const my_vec<T>& other) const {
		return !(*this == other);
	}

	/*!
		Removes an element from a vector by iterator
	*/
	vec_it<T> erase(vec_it<T> position) {
		for (vec_it<T> i = position; i != end() - 1; i++) {
			*i = *(i + 1);
		}
		_size--;
		return position;
	}

	/*!
		Clears the vector
	*/
	void clear() {
		if (capacity > 0) {
			delete[] data;
			data = nullptr;
			_size = 0;
			capacity = 0;
		}
	}

	friend class vec_it<T>;

	typedef vec_it<T> iterator;

	vec_it<T> begin() const { return vec_it<T>(data); }

	vec_it<T> end() const { return vec_it<T>(data + _size); }
};

template<class T>
class vec_it {
private:
	T *it;
public:
	vec_it() : it(0) {}

	vec_it(T *a) : it(a) {}

	vec_it(const vec_it<T> &p) { it = p.it; }

	~vec_it() {}

	vec_it<T> operator+(const size_t i) const {
		vec_it<T> res(*this);
		res.it += i;
		return res;
	}

	vec_it<T> operator-(const size_t i) const {
		vec_it<T> res(*this);
		res.it -= i;
		return res;
	}

	vec_it<T> &operator+=(const size_t i) {
		it += i;
		return *this;
	}

	vec_it<T> &operator-=(const size_t i) {
		it -= i;
		return *this;
	}

	vec_it<T> &operator++() {
		it++;
		return *this;
	}

	vec_it<T> operator++(int) {
		vec_it<T> res(*this);
		it++;
		return res;
	}

	vec_it<T> &operator--() {
		it--;
		return *this;
	}

	vec_it<T> operator--(int) {
		vec_it<T> res(*this);
		it--;
		return res;
	}

	bool operator<(const vec_it<T> a) const { return it < a.it; }

	bool operator>(const vec_it<T> a) const { return it > a.it; }

	bool operator<=(const vec_it<T> a) const { return it <= a.it; }

	bool operator>=(const vec_it<T> a) const { return it >= a.it; }

	bool operator!=(const vec_it<T> a) const { return it != a.it; }

	bool operator==(const vec_it<T> a) const { return it == a.it; }

	T &operator*() { return *it; }

	T *operator->() { return it; }
};
