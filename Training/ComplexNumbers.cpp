#include <iostream>

class Complex
{
public:
	Complex(int real, int imaginary = 0);
	friend Complex operator+(const Complex &c1, const Complex &c2);
	friend Complex operator-(const Complex &c1, const Complex &c2);
	friend Complex operator-(const Complex &c1);
	Complex &operator+=(const Complex &other);
	friend bool operator<(const Complex &c1, const Complex &c2);
	friend std::ostream &operator<<(std::ostream &os, const Complex &c);
	int get_real();
	int get_imaginary();

private:
	int m_Real, m_Imaginary;

};

Complex::Complex(int real, int imaginary)
: m_Real(real), m_Imaginary(imaginary) 
{
}

// Complex operator+(const Complex &c1, const Complex &c2)
// {
// 	return Complex(c1.m_Real + c2.m_Real, c1.m_Imaginary + c2.m_Imaginary);
// }

Complex operator+(const Complex &c1, const Complex &c2)
{
	Complex temp(c1);
	return temp += c2;
}

Complex operator-(const Complex &c1, const Complex &c2)
{
	return Complex(c1.m_Real - c2.m_Real, c1.m_Imaginary - c2.m_Imaginary);
}

Complex operator-(const Complex &c1)
{
	return Complex(-c1.m_Real, -c1.m_Imaginary);
}

Complex &Complex::operator+=(const Complex &other)
{
	m_Real += other.m_Real;
	m_Imaginary += other.m_Imaginary;
	return *this;
}

std::ostream &operator<<(std::ostream &os, const Complex &c)
{
	return os << "Complex( " << c.m_Real << ", " << c.m_Imaginary << " )" << std::endl;
}

bool operator<(const Complex &c1, const Complex &c2)
{
	return std::tie(c1.m_Real, c1.m_Imaginary) < std::tie(c2.m_Real, c2.m_Imaginary);
}

int Complex::get_real()
{
	return m_Real;
}

int Complex::get_imaginary()
{
	return m_Imaginary;
}

int main()
{
	Complex complex_1 (2, 2), complex_2 (1, 3);
	std::cout << complex_1 + complex_2 << std::endl;

	return 0;
}