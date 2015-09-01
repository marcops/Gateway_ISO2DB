#ifndef _ISO8583_H_
#define _ISO8583_H_


#define TAM_FORMAT_ISO 22 // TODO

#define MSG_VERYLONG_ISO "ISO MUITO GRANDE"
#define MSG_NO_ISOINIT	 "NAO ISO_INIT"


typedef enum {
	TB_FIX = 1,
	TB_LL  = 2,
	TB_LLL = 3	
}e_TypeBit;

typedef enum {
	ISO_IPIRANGA = 0,
	ISO_CORBAN,
	ISO_OTHERS
}e_TypeMap;


typedef struct {
	int bit;
	e_TypeBit type;
	int lenght;
	//char ome; // opcional('O') | mandatorio('M') | mandatorio echo ('E')
	char bna; // binario ('B') | numerico ('N') | alpha-numerico ('A')
}s_FormatIso;

/*
##	Layout default da ISO
*/
static const s_FormatIso defaultISO[TAM_FORMAT_ISO] = {		// completar -> ajustar TAM_FORMAT_ISO
	{  1, TB_FIX ,  16 , 'B' },
	{  2, TB_FIX ,   6 , 'N' },
	{  3, TB_FIX ,   6 , 'N' },
	{  7, TB_FIX ,  10 , 'N' },
	{ 11, TB_FIX ,   6 , 'N' },
	{ 12, TB_FIX ,   6 , 'N' },
	{ 13, TB_FIX ,   4 , 'N' },
	{ 28, TB_FIX ,   6 , 'N' },
	{ 32, TB_LL  ,  11 , 'N' },
	{ 35, TB_LL  ,  37 , 'N' },
	{ 39, TB_FIX ,   2 , 'N' },
	{ 41, TB_FIX ,   8 , 'A' },
	{ 42, TB_FIX ,  15 , 'A' },
	{ 43, TB_LL  ,  99 , 'N' },
	{ 45, TB_LL  ,  76 , 'N' },
	{ 48, TB_LLL , 999 , 'N' },
	{ 61, TB_LLL , 999 , 'N' },
	{ 62, TB_LLL , 999 , 'A' },
	{ 63, TB_LLL , 999 , 'N' },
	{ 71, TB_FIX ,   8 , 'N' },
	{125, TB_LLL ,   9 , 'N' },
	{127, TB_LLL ,   9 , 'N' }
};
/*static const s_FormatIso *defaultISO_Corban = {		
{  1, TB_FIX ,  16},
{  3, TB_FIX ,   6},
{  7, TB_FIX ,  10},
{ 11, TB_FIX ,   6},
{ 12, TB_FIX ,   6},
{ 13, TB_FIX ,   4},
{ 28, TB_FIX ,   6},
{ 32, TB_LL  ,  11},
{ 35, TB_LL  ,  37},
{ 39, TB_FIX ,   2},
{ 41, TB_FIX ,   8},
{ 42, TB_FIX ,  15},
{ 43, TB_LL  ,  99},
{ 45, TB_LL  ,  76},
{ 48, TB_LLL , 999},
{ 61, TB_LLL , 999},
{ 62, TB_LLL , 999},
{ 63, TB_LLL , 999},
{ 71, TB_FIX ,   8},
{125, TB_LLL ,   9},
{127, TB_LLL ,   9}
};*/

class Iso8583 {
public:
	//------------------------------------------------------------------
	//		bool ISO_Init(e_TypeMap Tmap);
	//		Inicia a ISO, deve ser utilizada antes das outras funcoes
	//		Parametros: Tmap: tipo de mapa de acordo com e_TypeMap
	//		Retorna Oo
	//-------------------------------------------------------------------
	Iso8583(e_TypeMap Tmap);// bool

	//------------------------------------------------------------------
	//		bool ISO_Extract( char *iso );
	//		Construtor de inico da extracao da ISO, deve ser utilizada antes do GET
	//		Parametros: iso: ponteiro para iso a ser quebrada
	//		Retorna Oo
	//-------------------------------------------------------------------
	bool Extract( const char *iso ); // bool

	//------------------------------------------------------------------
	//		bool ISO_Get( int pos , char *value );
	//		Funcao para buscar da ISO o valor de acordo com o bit
	//		Parametros: pos: bit do bitmap
	//					value: ponteiro onde sera retornado o valor desejado
	//		Retorna Oo
	//-------------------------------------------------------------------
	bool Get( int pos , char *value );

	//------------------------------------------------------------------
	//		bool ISO_CreateSend( char *iso );
	//		Construtor de inico para criacao da ISO, deve ser utilizada antes do SET
	//		Parametros: iso: ponteiro para iso a ser criada
	//					cod: codigo da mensagem Ex.: 0600
	//		Retorna Oo
	//-------------------------------------------------------------------
	bool CreateSend( char *iso, char cod[4] ); // bool

	//------------------------------------------------------------------
	//		bool ISO_Set( int pos , char *value );
	//		Descricao: Funcao para inserir na ISO o valor de acordo com o bit.
	//			Nao 'e possivel inserir bit 1, quando inserir bit > 64, bit 1 sera criado automaticamente.
	//		Parametros: pos: bit do bitmap
	//					value: ponteiro do valor a ser inserido
	//		Retorna Oo
	//-------------------------------------------------------------------
	bool Set( int pos,const char *value );


private:
	bool validaISO(void);
	bool PutValueInISO( int pos, const char *value );
	bool insereBitnaISO(int p);
	void Hex2Bits(void);
	void Hex2Bits2(void);
	bool confereDigito(int ini, int fim, char tp);


	e_TypeMap myTMap;

	bool didInit;
	bool didExtract;

	char myMap[129];
	char myMap2[129];
	const char *myISO;
	char *urISO;
	unsigned int TamISO;
	

};
#endif

