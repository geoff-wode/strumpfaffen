#if ! defined(__PROPERTY_H__)
#define __PROPERTY_H__

template <typename T> class Property
{
public:
	explicit Property(const T& value) : value(value) { }

	operator T() const { return value; }
	operator T&() { return value; }

	Property& operator=(const T& data) { value = data; return *this; }
	Property& operator=(const Property& other) { value = other.value; return *this; }

	operator T& () const { return data; }

private:
	T value;
	Property(const Property&);
};

#endif // __PROPERTY_H__
