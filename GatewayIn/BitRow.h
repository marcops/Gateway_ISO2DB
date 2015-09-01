#ifndef _BITROW_H_
#define _BITROW_H_

class BitRow {

public:
	string Bit() { return m_bit; }
	string SqlRow() { return m_sqlRow; }
	string SqlType() { return m_sqltype; }

	void Bit(string x) { m_bit = x;}
	void SqlRow(string x) { m_sqlRow = x;}
	void SqlType(string x) { m_sqltype = x;}

private:
	string m_bit;
	string m_sqlRow;
	string m_sqltype;
};

#endif