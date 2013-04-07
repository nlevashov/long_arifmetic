#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
using namespace std;

class NUM {
	bool _sign;
	size_t _len;
	std::vector<unsigned int> _figs;
	unsigned long long _radix;

	public:					//перед какими методами надо указывать const, а перед какими нет?
		NUM ();
		NUM (long long);
		NUM (const char *);
		NUM (const NUM &);

		NUM & operator = (const NUM &);
		NUM & operator = (long long);

		void scan ();
		void print () const;
	//	void scan (std::istream &);			//как организовать и как это влияет??
	//	std::ostream & print (std::ostream &);
		friend std::istream & operator >> (std::istream &, NUM &);
		friend std::ostream & operator << (std::ostream &, const NUM &);

		short comp_zero ();
		size_t getlen () const;
		NUM abs () const;
		NUM sqr () const;
		NUM cut (size_t) const;
		NUM operator >> (size_t) const;
		NUM operator << (size_t) const;
		NUM & operator >>= (size_t);
		NUM & operator <<= (size_t);

		bool operator == (long long) const;
		bool operator != (long long) const;
		bool operator <  (long long) const;
		bool operator <= (long long) const;
		bool operator >  (long long) const;
		bool operator >= (long long) const;

		bool operator == (const NUM &) const;
		bool operator != (const NUM &) const;
		bool operator <  (const NUM &) const;
		bool operator <= (const NUM &) const;
		bool operator >  (const NUM &) const;
		bool operator >= (const NUM &) const;

		const NUM & operator ++ ();
		const NUM operator ++ (int);
		const NUM & operator -- ();
		const NUM operator -- (int);

		NUM operator + (long long) const;
		NUM operator - (long long) const;
		NUM operator * (long long) const;
		NUM operator / (long long) const;
		NUM operator % (long long) const;

		NUM & operator += (long long);
		NUM & operator -= (long long);
		NUM & operator *= (long long);
		NUM & operator /= (long long);
		NUM & operator %= (long long);

		NUM operator + (const NUM &) const;
		NUM operator - (const NUM &) const;
		NUM operator * (const NUM &) const;
		NUM operator / (const NUM &) const;
		NUM operator % (const NUM &) const;

		NUM & operator += (const NUM &);
		NUM & operator -= (const NUM &);
		NUM & operator *= (const NUM &);
		NUM & operator /= (const NUM &);
		NUM & operator %= (const NUM &);

	private:
		char * readstr();
		void constructor(const char *);
		unsigned long long abs(long long) const;
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


NUM & NUM::operator = (const NUM & t)
{
	_sign = t._sign;
	_len = t._len;
	_figs = t._figs;
	_radix = t._radix;
	return *this;
}


NUM & NUM::operator = (long long p)
{
	*this = NUM(p);
	return *this;
}



//----input_output-------------------------------------

void NUM::scan()
{
	char * s = readstr(); //string.h
	constructor(s);
	free(s);
	_figs.resize(_len);
}


void NUM::print() const
{
/* natural form
	if (! _sign) std::cout << "- ";
	for (int i = 0; i < _len; i++) std::cout << _figs[i] << ' ';
	std::cout << endl << "len " << _len << endl;
*/

	NUM temp(*this);
	if (temp == 0) std::cout << '0';
	else {
		std::vector<char> ans;

		while (temp != 0) {
			ans.push_back((temp % 10)._figs[0]);
			temp /= 10;
		}

		if (! _sign) std::cout << '-';
		for (long long i = ans.size() - 1; i >= 0; i--) std::cout << ((int) ans[i]);
	}
}


std::istream & operator >> (std::istream & is, NUM & t)
{
	char s[101];
	is >> s;
	t.constructor(s);
	t._figs.resize(t._len);
	return is;
}


std::ostream & operator << (std::ostream & os, const NUM & t)
{
	NUM temp(t);
	if (temp == 0) os << '0';
	else {
		std::vector<char> ans;

		while (temp != 0) {
			ans.push_back((temp % 10)._figs[0]);
			temp /= 10;
		}

		if (! temp._sign) os << '-';
		for (long long i = ans.size() - 1; i >= 0; i--) os << ((int) ans[i]);
	}

	return os;
}



//----sign_and_length_work-----------------------------

short NUM::comp_zero()
{
	if ((_len == 1) && (_figs[0] == 0)) return 0;
	else if (_sign) return 1;
	else return -1;
}


size_t NUM::getlen() const
{
	return _len;
}


NUM NUM::abs () const
{
	NUM ans(*this);
	ans._sign = true;
	return ans;
}


NUM NUM::sqr () const
{
	if (_len == 1) {
		NUM ans(_figs[0]);
		ans *= _figs[0];
		ans._sign = true;
		return ans;
	}

	size_t n = (_len / 2) + (_len % 2);
	NUM A = cut(n);
	NUM B = *this >> n;

	NUM ans(A.sqr() + (((A * B) << n) * 2) + (B.sqr() << (n * 2)));
	return ans;
}


NUM NUM::cut (size_t l) const
{
	NUM ans(*this);	//плохо

	if (_len > l) {
		ans._figs.resize(l);
		ans._len = l;
	}

	return ans;
}


NUM NUM::operator >> (size_t l) const
{
	if (_len <= l) return NUM((long long) 0);

	NUM ans;
	ans._figs.resize(_len - l);
	ans._sign = _sign;
	for (size_t i = 0; i < _len - l; i++) ans. _figs[i] = _figs[i + l];
	ans._len = _len - l;

	return ans;
}


NUM NUM::operator << (size_t l) const
{
	if ((_len == 1) && (_figs[0] == 0)) {
		return NUM((long long) 0);
	}

	NUM ans;
	ans._sign = _sign;
	for (size_t i = 0; i < l; i++) ans._figs.push_back(0);
	for (size_t i = 0; i < _len; i++) ans. _figs.push_back(_figs[i]);
	ans._len = _len + l;

	return ans;
}


NUM & NUM::operator >>= (size_t l)
{
	return (*this = *this >> l);
}


NUM & NUM::operator <<= (size_t l)
{
	return (*this = *this << l);
}



//----comparisons--------------------------------------

bool NUM::operator == (long long t) const
{
	unsigned long long abst = abs(t);
	bool signt = (t > 0);
	if ((_len == 1) && (_figs[0] == 0)) return (t == 0);
	if ( (_sign != signt) || (_len > 2) ) return false;
	if (_figs[0] != (abst % _radix)) return false;
	if (_len == 1) return true;
	return (_figs[1] == (abst / _radix));
}


bool NUM::operator != (long long t) const
{
	return (! (*this == t));
}


bool NUM::operator < (long long t) const
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
bool NUM::operator <= (long long t) const
{
	return ((*this < t) || (*this == t));
}


bool NUM::operator > (long long t) const
{
	return (! (*this <= t));
}


bool NUM::operator >= (long long t) const
{
	return (! (*this < t));
}



bool NUM::operator == (const NUM & t) const
{
	if ((_len == 1) && (_figs[0] == 0)) return ((t._len == 1) && (t._figs[0] == 0));
	if (_sign != t._sign) return false;
	if (_len != t._len) return false;

	for (size_t i = 0; i < _len; i++)
		if (_figs[i] != t._figs[i]) return false;

	return true;
}


bool NUM::operator != (const NUM & t) const
{
	return (! (*this == t));
}



bool NUM::operator < (const NUM & t) const
{
	if ((_len == 1) && (_figs[0] == 0)) {
		if ((t._len == 1) && (t._figs[0] == 0)) return false;
		return t._sign;
	}
	if (_sign != t._sign) return (_sign < t._sign);
	if (_len != t._len) return (_len < t._len);

	for (size_t i = _len - 1; i > 0; i--) {			//переделать цикл!
		if (_figs[i] < t._figs[i]) return true;
		if (_figs[i] > t._figs[i]) return false;
	}

	return (_figs[0] < t._figs[0]);
}


//следующие три можно написать отдельно для ускорения
bool NUM::operator <= (const NUM & t) const
{
	return ((*this < t) || (*this == t));
}


bool NUM::operator > (const NUM & t) const
{
	return (! (*this <= t));
}


bool NUM::operator >= (const NUM & t) const
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

NUM NUM::operator + (long long p) const
{
	NUM ans(*this);
	return (ans += p);
}


NUM NUM::operator - (long long p) const
{
	NUM ans(*this);
	return (ans -= p);
}


NUM NUM::operator * (long long p) const
{
	NUM ans(*this);
	return (ans *= p);
}



NUM NUM::operator / (long long p) const
{
	NUM ans(*this);
	return (ans /= p);
}


NUM NUM::operator % (long long p) const
{
	NUM ans(*this);
	return (ans %= p);
}



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


NUM & NUM::operator /= (long long p)
{
	if (abs(p) >= _radix) return (*this /= NUM(p));

	_sign = (_sign == (p > 0));
	p = abs(p);

	long long next = _figs[_len - 1];

	for (size_t i = _len - 1; i > 0; i--) {
		_figs[i] = next / p;
		next = (next % p) * _radix + _figs[i-1];
	}

	_figs[0] = next / p;

	while ((_len > 1) && (_figs[_len - 1] == 0)) {
		_figs.pop_back();
		_len--;
	}

	return *this;
}

NUM & NUM::operator %= (long long p)
{
	if (abs(p) >= _radix) return (*this %= NUM(p));

	_sign = (_sign == (p > 0));
	p = abs(p);

	long long next = _figs[_len - 1];

	for (size_t i = _len - 1; i > 0; i--) {
		_figs[i] = next / p;
		next = (next % p) * _radix + _figs[i-1];
	}

	_figs[0] = next % p;

	_figs.resize(1);
	_len = 1;

	return *this;
}




//----long_arifmetic-----------------------------------

NUM NUM::operator + (const NUM & p) const
{
	NUM ans(*this);
	return (ans += p);
}


NUM NUM::operator - (const NUM & p) const
{
	NUM ans(*this);
	return (ans -= p);
}


NUM NUM::operator * (const NUM & p) const
{
	if (_len == 1) {
		NUM ans(p);
		ans *= _figs[0];
		ans._sign = (p._sign == _sign);
		return ans;
	}
	if (p._len == 1) {
		NUM ans(*this);
		ans *= p._figs[0];
		ans._sign = (p._sign == _sign);
		return ans;
	}

	size_t n = (_len / 2) + (_len % 2);
	NUM A1 = cut(n);
	NUM B1 = *this >> n;

	size_t m = (p._len / 2) + (p._len % 2);
	NUM A2 = p.cut(m);
	NUM B2 = p >> m;

	return NUM((A1 * A2) + ((A1 * B2) << m) + ((A2 * B1) << n) + ((B1 * B2) << (n + m)));
}


NUM NUM::operator / (const NUM & p) const
{
	NUM absthis = this -> abs(), absp = p.abs();
	if (absthis < absp) return NUM((long long) 0);

	bool sign = (_sign == p._sign);

	NUM low;
	low._len = (_len - p._len > 0 ? _len - p._len : 1);
	low._sign = true;
	for (size_t i = 0; i < low._len - 1; i++) low._figs.push_back(0);
	low._figs.push_back(1);

	NUM high;
	high._len = _len - p._len + 1;
	high._sign = true;
	for (size_t i = 0; i < high._len; i++) high._figs.push_back(_radix - 1);

	while ((low + 1) < high) { // < or != ???
		NUM mid = ((low + high) / 2);
		if (absthis < (mid * absp)) high = mid;
		else low = mid;
	}

	low._sign = sign;
	return low;
}


NUM NUM::operator % (const NUM & p) const
{
	NUM ans(*this);
	return (ans %= p);
}

NUM & NUM::operator += (const NUM & p)
{
	if ( (_sign) == (p._sign) ) return big_sum(p);
	else {
		NUM * t = new NUM(p);
		if (this -> abs() < t -> abs()) {
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
		if (this -> abs() < t -> abs()) {
			t -> _sign = ! t -> _sign;
			*this = t -> big_sub(*this);
			return *this;
		} else return big_sub(p);
	}
}


NUM & NUM::operator *= (const NUM & p)
{
	return (*this = *this * p);
}


NUM & NUM::operator /= (const NUM & p)
{
	return (*this = *this / p);
}

NUM & NUM::operator %= (const NUM & p)
{
	bool sign = (_sign == p._sign);
	this -> _sign = true;
	NUM absp = p.abs();

	if (*this < absp) {
		*this = sign;
		return *this;
	}

	NUM low;
	low._len = (_len - p._len > 0 ? _len - p._len : 1);
	low._sign = true;
	for (size_t i = 0; i < low._len - 1; i++) low._figs.push_back(0);
	low._figs.push_back(1);

	NUM high;
	high._len = _len - p._len + 1;
	high._sign = true;
	for (size_t i = 0; i < high._len; i++) high._figs.push_back(_radix - 1);

	while ((low + 1) < high) { // < or != ???
		NUM mid = ((low + high) / 2);
		if (*this < (mid * absp)) high = mid;
		else low = mid;
	}

	*this -= low * absp;
	this -> _sign = sign;
	return *this;
}



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


unsigned long long NUM::abs(long long t) const
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
	ifstream fin("INPUT.TXT");
	ofstream fout("OUTPUT.TXT");

	int n;
	NUM a = 1;

	fin >> n;

	for (int i = 0; i < n; i++) a *= 45;

	fout << a;

	fin.close();
	fout.close();

	return 0;
}
