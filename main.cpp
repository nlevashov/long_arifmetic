#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
using namespace std;

class NUM {
	bool _sign;
	size_t _len;
	vector<unsigned int> _figs;
	unsigned long long _radix;

	public:
		NUM ();
		NUM (long long);
		NUM (const char *);
		NUM (const NUM &);

		NUM & operator = (const NUM &);

		std::istream & operator >> (std::istream &);
		std::ostream & operator << (std::ostream &);
		void scan ();
		void print () const;

		short comp_zero ();
		size_t getlen ();
		NUM & getabs ();
		NUM & sqr();
		NUM & cut (size_t) const;
		NUM & operator >> (size_t) const;
		NUM & operator << (size_t) const;

		bool operator == (long long);
		bool operator != (long long);
		bool operator <  (long long);
		bool operator <= (long long);
		bool operator >  (long long);
		bool operator >= (long long);

		bool operator == (const NUM &);
		bool operator != (const NUM &);
		bool operator <  (const NUM &);
		bool operator <= (const NUM &);
		bool operator >  (const NUM &);
		bool operator >= (const NUM &);

		const NUM & operator ++ ();
		const NUM operator ++ (int);
		const NUM & operator -- ();
		const NUM operator -- (int);

		NUM & operator + (long long);
		NUM & operator - (long long);
		NUM & operator * (long long);
		NUM & operator / (long long);
//		NUM & operator % (long long);

		NUM & operator += (long long);
		NUM & operator -= (long long);
		NUM & operator *= (long long);
//		NUM & operator /= (long long);
//		NUM & operator %= (long long);

		NUM & operator + (const NUM &);
		NUM & operator - (const NUM &);
		NUM & operator * (const NUM &);
//		NUM & operator / (const NUM &);
//		NUM & operator % (const NUM &);

		NUM & operator += (const NUM &);
		NUM & operator -= (const NUM &);
//		NUM & operator *= (const NUM &);
//		NUM & operator /= (const NUM &);
//		NUM & operator %= (const NUM &);

	private:
		char * readstr();
		void constructor(const char *);
		unsigned long long abs(long long);
		NUM & short_sum(unsigned long long);
		NUM & short_sub(unsigned long long);
		NUM & big_sum(const NUM & p);
		NUM & big_sub(const NUM & p);
};



//----constructors-------------------------------------

NUM::NUM ()
{
	_len = 0;
	_radix = (((unsigned long long) 1) << ((unsigned char) sizeof(unsigned int) * 8));
}

NUM::NUM (long long p)
{
	_radix = (((unsigned long long) 1) << ((unsigned char) sizeof(unsigned int) * 8));
	_sign = (p >= 0);
	p = abs(p);
	_figs.push_back(p % _radix);
	if (p / _radix > 0) _figs.push_back(p / _radix);
	_len = _figs.size();
}


NUM::NUM (const char * s)
{
	_radix = (((unsigned long long) 1) << ((unsigned char) sizeof(unsigned int) * 8));
	constructor(s);
}


NUM::NUM (const NUM & t)
{
	_sign = t._sign;
	_len = t._len;
	_figs = t._figs;
	_radix = t._radix;
}


NUM & NUM::operator = (const NUM & t) {
	_sign = t._sign;
	_len = t._len;
	_figs = t._figs;
	_radix = t._radix;
	return *this;
}



//----input_output-------------------------------------

std::istream & operator >> (std::istream & is, NUM & t)
{
	t.scan();
	return is;
}


std::ostream & operator << (std::ostream & os, const NUM & t)
{
	t.print();
	return os;
}


void NUM::scan()
{
	char * s = readstr(); //string.h
	constructor(s);
	free(s);
	_figs.resize(_len);
}


void NUM::print() const
{
	if (! _sign) cout << "- ";
	for (int i = 0; i < _len; i++) cout << _figs[i] << ' ';
	cout << endl << "len " << _len << endl;
}




//----sign_and_length_work-----------------------------

short NUM::comp_zero()
{
	if ((_len == 1) && (_figs[0] == 0)) return 0;
	else if (_sign) return 1;
	else return -1;
}


size_t NUM::getlen()
{
	return _len;
}


NUM & NUM::getabs()
{
	NUM * ans = new NUM(*this);
	ans -> _sign = true;
	return *ans;
}


NUM & NUM::sqr ()
{
	if (_len == 1) {
		NUM * ans = new NUM(_figs[0]);
		*ans *= _figs[0];
		ans->_sign = true;
		return *ans;
	}

	size_t n = (_len / 2) + (_len % 2);
	NUM A = cut(n);
	NUM B = *this >> n;

	NUM * res = new NUM(A.sqr() + (((A * B) << n) * 2) + (B.sqr() << (n * 2)));
	return *res;
}


NUM & NUM::cut (size_t l) const
{
	NUM * ans = new NUM(*this);	//плохо

	if (_len > l) {
		ans -> _figs.resize(l);
		ans -> _len = l;
	}

	return *ans;
}


NUM & NUM::operator >> (size_t l) const
{
	if (_len <= l) return *(new NUM((long long) 0));

	NUM * ans = new NUM();
	ans -> _figs.resize(_len - l);
	ans -> _sign = _sign;
	for (size_t i = 0; i < _len - l; i++) ans -> _figs[i] = _figs[i + l];
	ans -> _len = _len - l;

	return *ans;
}


NUM & NUM::operator << (size_t l) const
{
	if ((_len == 1) && (_figs[0] == 0)) {
		return *(new NUM((long long) 0));
	}

	NUM * ans = new NUM();
	ans->_sign = _sign;
	for (size_t i = 0; i < l; i++) ans->_figs.push_back(0);
	for (size_t i = 0; i < _len; i++) ans -> _figs.push_back(_figs[i]);
	ans -> _len = _len + l;

	return *ans;
}



//----comparisons--------------------------------------

bool NUM::operator == (long long t)
{
	unsigned long long abst = abs(t);
	bool signt = (t > 0);
	if ((_len == 1) && (_figs[0] == 0)) return (t == 0);
	if ( (_sign != signt) || (_len > 2) ) return false;
	if (_figs[0] != (abst % _radix)) return false;
	if (_len == 1) return true;
	return (_figs[1] == (abst / _radix));
}


bool NUM::operator != (long long t)
{
	return (! (*this == t));
}


bool NUM::operator < (long long t)
{
	unsigned long long abst = abs(t);
	bool signt = (t > 0);
	if ((_len == 1) && (_figs[0] == 0)) return signt;
	if (_sign != signt) return signt;
	if (_len > 2) return (! _sign);
	if (_len == 1) {
		if ((abst / _radix) == 0) return ((_figs[0] < (abst % _radix)) == _sign);
		else return _sign;
	}
	if (_sign) return ( (_figs[1] < (abst / _radix)) || ((_figs[1] == (abst / _radix)) && (_figs[0] < (abst % _radix))) );
	return ( (_figs[1] > (abst / _radix)) || ((_figs[1] == (abst / _radix)) && (_figs[0] > (abst % _radix))) );
}


//следующие три можно написать отдельно для ускорения
bool NUM::operator <= (long long t)
{
	return ((*this < t) || (*this == t));
}


bool NUM::operator > (long long t)
{
	return (! (*this <= t));
}


bool NUM::operator >= (long long t)
{
	return (! (*this < t));
}



bool NUM::operator == (const NUM & t)
{
	if ((_len == 1) && (_figs[0] == 0)) return ((t._len == 1) && (t._figs[0] == 0));
	if (_sign != t._sign) return false;
	if (_len != t._len) return false;

	for (size_t i = 0; i < _len; i++)
		if (_figs[i] != t._figs[i]) return false;

	return true;
}


bool NUM::operator != (const NUM & t)
{
	return (! (*this == t));
}


bool NUM::operator < (const NUM & t)
{
	if ((_len == 1) && (_figs[0] == 0)) {
		if ((t._len == 1) && (t._figs[0] == 0)) return false;
		return t._sign;
	}
	if (_sign != t._sign) return (_sign < t._sign);
	if (_len != t._len) return (_len < t._len);

	for (size_t i = 0; i < _len; i++)
		if (_figs[i] >= t._figs[i]) return false;

	return true;
}


//следующие три можно написать отдельно для ускорения
bool NUM::operator <= (const NUM & t)
{
	return ((*this < t) || (*this == t));
}


bool NUM::operator > (const NUM & t)
{
	return (! (*this <= t));
}


bool NUM::operator >= (const NUM & t)
{
	return (! (*this < t));
}



//----inc_dec------------------------------------------

const NUM & NUM::operator ++ ()
{
	*this += 1;
	return *this;
}


const NUM NUM::operator ++ (int i)
{
	NUM ans(*this);
	*this += 1;
	return ans;
}


const NUM & NUM::operator -- ()
{
	*this -= 1;
	return *this;
}


const NUM NUM::operator -- (int i)
{
	NUM ans(*this);
	*this -= 1;
	return ans;
}



//----short_arifmetic----------------------------------

NUM & NUM::operator + (long long p)
{
	NUM * ans = new NUM(*this);
	return (*ans += p);
}


NUM & NUM::operator - (long long p)
{
	NUM * ans = new NUM(*this);
	return (*ans -= p);
}


NUM & NUM::operator * (long long p)
{
	NUM * ans = new NUM(*this);
	return (*ans *= p);
}



NUM & NUM::operator / (long long p)
{
	NUM * ans = new NUM (*this);

	bool ans_sign = ans -> _sign;
	ans -> _sign = true;
	if (*ans < abs(p)) return (*ans *= 0);

	
}

//		NUM & operator % (long long);



NUM & NUM::operator += (long long p)
{
	if ( (_sign) == (p >= 0) ) return short_sum(abs(p));
	else return short_sub(abs(p));
}


NUM & NUM::operator -= (long long p)
{
	if ( (_sign) != (p >= 0) ) return short_sum(abs(p));
	else return short_sub(abs(p));
}


NUM & NUM::operator *= (long long p)
{
	if (p == 0) {
		_figs.resize(1);
		_len = 1;
		_figs[0] = 0;
	}

	unsigned long long t = 0, j = 0;

	while (j < _len) {
		t = t / _radix + _figs[j] * p;
		_figs[j] = t % _radix;
		j++;
	}

	if (t >= _radix) {
		_figs.push_back(t / _radix);
		_len++;
	}

	_sign = (_sign == (p > 0));

	return *this;
}


//NUM & NUM::operator /= (long long p)
//NUM & operator %= (long long);




//----long_arifmetic-----------------------------------

NUM & NUM::operator + (const NUM & p)
{
	NUM * ans = new NUM(*this);
	return (*ans += p);
}


NUM & NUM::operator - (const NUM & p)
{
	NUM * ans = new NUM(*this);
	return (*ans -= p);
}


NUM & NUM::operator * (const NUM & p)
{
	if (_len == 1) {
		NUM * ans = new NUM(p);
		*ans *= _figs[0];
		ans->_sign = (p._sign == _sign);
		return *ans;
	}
	if (p._len == 1) {
		NUM * ans = new NUM(*this);
		*ans *= p._figs[0];
		ans->_sign = (p._sign == _sign);
		return *ans;
	}

	size_t n = (_len / 2) + (_len % 2);
	NUM A1 = cut(n);
	NUM B1 = *this >> n;

	size_t m = (p._len / 2) + (p._len % 2);
	NUM A2 = p.cut(m);
	NUM B2 = p >> m;

	NUM * res = new NUM((A1 * A2) + ((A1 * B2) << m) + ((A2 * B1) << n) + ((B1 * B2) << (n + m)));
	return *res;
}


//		NUM & operator / (const NUM &);
//		NUM & operator % (const NUM &);

NUM & NUM::operator += (const NUM & p)
{
	if ( (_sign) == (p._sign) ) return big_sum(p);
	else {
		NUM * t = new NUM(p);
		if (this -> getabs() < t -> getabs()) {
			*this = t -> big_sub(*this);
			return *this;
		} else return big_sub(p);
	}
}


NUM & NUM::operator -= (const NUM & p)
{
	if ( (_sign) != (p._sign) ) return big_sum(p);
	else {
		NUM * t = new NUM(p);
		if (this -> getabs() < t -> getabs()) {
			t -> _sign = ! t -> _sign;
			*this = t -> big_sub(*this);
			return *this;
		} else return big_sub(p);
	}
}


//		NUM & operator *= (const NUM &);
//		NUM & operator /= (const NUM &);
//		NUM & operator %= (const NUM &);



//----private_functions--------------------------------

char * NUM::readstr()
{
	unsigned long long n = 64;
	char * s = (char *) malloc(n);
	if (s == NULL) {
		printf("Ошибка выделения памяти\n");
		exit(EXIT_FAILURE);
	}

	unsigned long long i = 0;
	s[0] = getchar();
	if (s[0] == '-') {
		i = 1;
		s[1] = getchar();
	}
	while (s[i] >= '0' && s[i] <= '9') {
		if (i == n - 1) {
			char * sTemp = (char *) realloc(s, n << 1);
			if (sTemp == NULL) {
				printf("Ошибка выделения памяти\n");
				free(s);
				exit(EXIT_FAILURE);
			}
			s = sTemp;
			n <<= 1;
		}
		i++;
		s[i] = getchar();
	}
	s[i] = '\0';

	return s;
}


void NUM::constructor(const char * s)
{
	unsigned long long
		slen = strlen(s),
		_radix = (((unsigned long long) 1) << ((unsigned char) sizeof(unsigned int) * 8));

	unsigned char inf;
	bool sign;
	if (s[0] == '-') {
		sign = false;
		inf = 1;
	} else {
		sign = true;
		inf = 0;
	}

	_figs.push_back(s[inf] - '0');
	_len = 1;
	_sign = true;
	for (unsigned long long i = inf + 1; i < slen; i++) {
		*this *= 10;
		*this += s[i] - '0';
	}
	_sign = sign;
}


unsigned long long NUM::abs(long long t)
{
	if (t > 0) return t;
	else return -t;
}


NUM & NUM::short_sum(unsigned long long p)
{
	unsigned long long t = _figs[0] + p;
	size_t i = 0;

	while ((i + 1 < _len) && (t >= _radix)) {
		_figs[i] = t % _radix;
		i++;
		t = t / _radix + _figs[i];
	}

	_figs[i] = t % _radix;
	t /= _radix;

	if ((i + 1 == _len) && (t > 0)) {
		_figs.push_back(t);
		_len++;
	}

	return *this;
}


NUM & NUM::short_sub(unsigned long long p)
{
	long long t = p - _figs[0];
	size_t i = 0;

	while ((i + 1 < _len) && (t > 0)) {
		_figs[i] = _radix - (t % _radix);
		i++;
		t = t / _radix - _figs[i] + (t % _radix == 0 ? 0 : 1);
	}

	if (t <= 0) _figs[i] = -t;

	if ((i + 1 == _len) && (t > 0)) {
		_figs[i] = t % _radix;
		_figs.push_back(t / _radix);
		_len++;
		_sign = ! _sign;
	}

	i = _len - 1;
	while ((_figs[i] == 0) && (i > 0)) {
		_figs.pop_back();
		i--;
	}
	_len = i + 1;

	return *this;
}


NUM & NUM::big_sum(const NUM & p)
{
	unsigned long long t = 0;
	size_t minlen = (_len < p._len) ? _len : p._len ;

	for (size_t i = 0; i < minlen; i++) {
		t = t / _radix + _figs[i] + p._figs[i];
		_figs[i] = t % _radix;
	}

	for (size_t i = minlen; i < _len; i++) {
		t = t / _radix + _figs[i];
		_figs[i] = t % _radix;
	}

	for (size_t i = minlen; i < p._len; i++) {
		t = t / _radix + p._figs[i];
		_figs.push_back(t % _radix);
		_len++;
	}

	t /= _radix;
	if (t > 0) {
		_figs.push_back(t);
		_len++;
	}

	return *this;
}


NUM & NUM::big_sub(const NUM & p)
{
	long long t;
	unsigned char duty = 0;

	for (size_t i = 0; i < p._len; i++) {
		t = (long long) _figs[i] - p._figs[i] - duty;
		if (t < 0) {
			_figs[i] = _radix + t;
			duty = 1;
		} else {
			_figs[i] = t;
			duty = 0;
		}
	}

	for (size_t i = p._len; i < _len; i++) {
		t = (long long) _figs[i] - duty;
		if (t < 0) {
			_figs[i] = _radix + t;
			duty = 1;
		} else {
			_figs[i] = t;
			duty = 0;
		}
	}

	size_t i = _len - 1;
	while ((_figs[i] == 0) && (i > 0)) {
		_figs.pop_back();
		i--;
	}
	_len = i + 1;

	return *this;
}



int main ()
{
	NUM a;
	NUM b;
	cin >> a >> b;
	cout << a << b;
	cout << (a*b);
//	cout << a << a.cut(3) << (a >> 2) << (a << 4);
//	cout << a << b << (a + b) << (a + t);
//	cout << (a < b) << endl;
//	c = b = a;
/*	cout << "(a <= b) = " << (a <= b) << endl;
	cout << "(a >  b) = " << (a > b) << endl;
	cout << "(a >= b) = " << (a >= b) << endl;
	cout << (a <= 0) << (a > 0) << (a >= 0) << endl;
	cout << (a <= -0) << (a > -0) << (a >= -0) << endl;
	cout << (a <= 123) << (a > 123) << (a >= 123) << endl;
	cout << (a <= -567) << (a > -567) << (a >= -567) << endl;
	cout << (a <= 4294967297) << (a > 4294967297) << (a >= 4294967297) << endl;
	cout << (a <= -4294967298) << (a > -4294967298) << (a >= -4294967298) << endl;
	cout << (a <= 9223372036854775807) << (a > 9223372036854775807) << (a >= 9223372036854775807) << endl;
	cout << (a <= -9223372036854775807) << (a > -9223372036854775807) << (a >= -9223372036854775807) << endl;
*/
//	a += 9223372036854775807;
//	b *= 100;
//	NUM b("-10");

//	NUM c = a + b;
//	a += b;
//	c -= b;
//	b = b - 1000;

//	a.print();
//	b.print();
//	c.print();

	return 0;
}
