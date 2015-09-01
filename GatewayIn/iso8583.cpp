#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "iso8583.h"

static const unsigned int inv_hex[] = {8,4,2,1};

Iso8583::Iso8583(e_TypeMap Tmap)
{
	myTMap = Tmap;
	didInit = true;
	didExtract = true;
	TamISO = 0;
}

//extrai
bool Iso8583::Extract( const char *iso )
{
	if(didInit == false) return false;
	else myISO = iso;	
	didExtract = true;
	Hex2Bits();
	return validaISO();
	//return true;
}

bool Iso8583::Get( int pos , char *value )
{
	int i,soma, elles, tam;
	char aux[3] = "";

	if(didInit == false) return false;
	else if(didExtract == false) return false;
	else 
	{		
		//printf("%s\n", myMap);
		//if(myMap[1] == '1')soma = 20; // codigo da msg + map1 
		//else soma = 4;
		soma = 20;

		if(myTMap == ISO_IPIRANGA)
		{
			for(i=0; i<TAM_FORMAT_ISO; i++) 
			{		
				if (myMap[defaultISO[i].bit] == '1')
				{
					//printf("%d\n", defaultISO[i].bit);
					if(defaultISO[i].type == TB_FIX)
					{
						tam = defaultISO[i].lenght;
						elles = 0;
					}else if(defaultISO[i].type == TB_LL)
					{
						elles = 2;
						strncpy (aux, myISO+soma, 2);
						tam = atoi(aux) ;
					}else if(defaultISO[i].type == TB_LLL)
					{
						elles = 3;
						strncpy (aux, myISO+soma, 3);						
						tam = atoi(aux) ;
					}

					if (defaultISO[i].bit == pos)
					{								
						strncpy (value, myISO+soma+elles,tam);
						value[tam] = '\0';
						return true;
					}			

					soma = soma + tam + elles;	
				}
			}
		}
	}
	return false;
}

//cria

bool Iso8583::CreateSend( char *iso, char cod[4] ) // falta o codigo da msg ex.: 0600
{		
	if(didInit == false) return false;
	else urISO = iso;	
	didExtract = true;
	strcpy(urISO,cod);
	memset(urISO+4, '0', 16);
	//urISO[4] = '3';
	urISO[20] = '\0';	
	TamISO = 20;
	//printf("%s\n", urISO);
	//printf("%d\n", strlen(urISO));
	return true;
}

bool Iso8583::Set( int pos,const char *value ){


	if(pos == 1) return false; // colocar map 2
	else{
		if(pos > 64 && !(urISO[4] > '7')) // nao possui dois maps e colocou bit do segundo map
		{
			if( insereBitnaISO(1) == false) return false;
			Hex2Bits2();
			PutValueInISO(1 ,"0000000000000000");
			if( insereBitnaISO(pos) == false) return false;
			Hex2Bits2();
			PutValueInISO(pos ,value);
		}else
		{
			if( insereBitnaISO(pos) == false) return false;
			Hex2Bits2();
			if( PutValueInISO(pos ,value) == false) return false;
		}	
		return true;
	}
}

bool Iso8583::PutValueInISO( int pos, const char *value )
{
	int elles, tamVal = 0;
	int i;
	int soma = 20;
	char aux_elles[4];


	//if(urISO[1] > '7') soma = soma + 16; // possui 2 map

	tamVal = strlen(value);

	for(i=0; i<TAM_FORMAT_ISO; i++) 
	{						

		if(defaultISO[i].bit == pos) // encontro na tabela pelo layout do bit
		{	
			if(defaultISO[i].lenght < (int)strlen(value))return false;
			else
			{	
				memset(aux_elles, 0 , sizeof(aux_elles));
				if( defaultISO[i].type == TB_FIX ) elles = 0;
				else 
					if( defaultISO[i].type == TB_LL )
					{
						sprintf(aux_elles, "%02u", strlen(value));
						elles = 2;
					}
					else
					{
						sprintf(aux_elles, "%03u", strlen(value));
						elles = 3;
					}

					//memmove(urISO+soma+strlen(value), urISO+soma, TamISO - soma);				
					//memmove(urISO+soma, value, strlen(value));
					strncpy(urISO+soma+strlen(value)+elles, urISO+soma, TamISO - soma);
					if(elles>0)memmove(urISO+soma, aux_elles, strlen(aux_elles));
					strncpy(urISO+soma+elles, value, strlen(value));

					TamISO += tamVal + elles;
					urISO[TamISO] = '\0';
					return true;
			}

		}	
		if (myMap2[defaultISO[i].bit] == '1') // consta no map
		{				
			if( defaultISO[i].type == TB_FIX ) elles = 0;
			else if( defaultISO[i].type == TB_LL ) elles = 2;
			else elles = 3;

			if(elles > 0)
			{
				strncpy(aux_elles, urISO+soma, elles);
				soma = soma + atoi(aux_elles) + elles; 
			}else soma = soma + defaultISO[i].lenght + elles; 
		}

	}
	return true;
}

bool Iso8583::insereBitnaISO(int p)
{
	int div, resto = 0;
	char aux[2];
	memset(aux, 0,sizeof(aux));

	div = p / 4;
	resto = p % 4;

	if(myMap2[p] == '1')return false;

	if (resto == 0) 
	{
		resto = 4;
		div --;
	}
	div = div + 4;
	sprintf(aux,"%X",urISO[div]+inv_hex[resto-1]-48); // 2 do bit, 48 do asc

	urISO[div] = aux[0];
	return true;
}

void Iso8583::Hex2Bits(void)
{    	
	int j , i, maps;
	j = 1;
	myMap[0] = 'X';
	if(myISO[4] > '7') maps = 36; // 2 mapas
	else maps = 20;

	for(i = 4 ; i < maps ; i++)
	{
		switch(myISO[i])
		{      
		case '0': myMap[j] = '0'; myMap[j+1] = '0'; myMap[j+2] = '0'; myMap[j+3] = '0';break;
		case '1': myMap[j] = '0'; myMap[j+1] = '0'; myMap[j+2] = '0'; myMap[j+3] = '1';break; 
		case '2': myMap[j] = '0'; myMap[j+1] = '0'; myMap[j+2] = '1'; myMap[j+3] = '0';break;
		case '3': myMap[j] = '0'; myMap[j+1] = '0'; myMap[j+2] = '1'; myMap[j+3] = '1';break;
		case '4': myMap[j] = '0'; myMap[j+1] = '1'; myMap[j+2] = '0'; myMap[j+3] = '0';break;
		case '5': myMap[j] = '0'; myMap[j+1] = '1'; myMap[j+2] = '0'; myMap[j+3] = '1';break;
		case '6': myMap[j] = '0'; myMap[j+1] = '1'; myMap[j+2] = '1'; myMap[j+3] = '0';break;
		case '7': myMap[j] = '0'; myMap[j+1] = '1'; myMap[j+2] = '1'; myMap[j+3] = '1';break;
		case '8': myMap[j] = '1'; myMap[j+1] = '0'; myMap[j+2] = '0'; myMap[j+3] = '0';break;
		case '9': myMap[j] = '0'; myMap[j+1] = '0'; myMap[j+2] = '0'; myMap[j+3] = '1';break;
		case 'A': myMap[j] = '1'; myMap[j+1] = '0'; myMap[j+2] = '1'; myMap[j+3] = '0';break;
		case 'B': myMap[j] = '1'; myMap[j+1] = '0'; myMap[j+2] = '1'; myMap[j+3] = '1';break;
		case 'C': myMap[j] = '1'; myMap[j+1] = '1'; myMap[j+2] = '0'; myMap[j+3] = '0';break;
		case 'D': myMap[j] = '1'; myMap[j+1] = '1'; myMap[j+2] = '0'; myMap[j+3] = '1';break;
		case 'E': myMap[j] = '1'; myMap[j+1] = '1'; myMap[j+2] = '1'; myMap[j+3] = '0';break;
		case 'F': myMap[j] = '1'; myMap[j+1] = '1'; myMap[j+2] = '1'; myMap[j+3] = '1';break;    
		}   
		j = j + 4;
	}
}

// atualiza mapa de bits
void Iso8583::Hex2Bits2(void) 
{    	
	int j , i, maps;
	j = 1;
	myMap2[0] = 'X';
	if(urISO[4] > '7') maps = 36; // 2 mapas
	else maps = 20;

	for(i = 4 ; i < maps ; i++)
	{
		switch(urISO[i])
		{      
		case '0': myMap2[j] = '0'; myMap2[j+1] = '0'; myMap2[j+2] = '0'; myMap2[j+3] = '0';break;
		case '1': myMap2[j] = '0'; myMap2[j+1] = '0'; myMap2[j+2] = '0'; myMap2[j+3] = '1';break;
		case '2': myMap2[j] = '0'; myMap2[j+1] = '0'; myMap2[j+2] = '1'; myMap2[j+3] = '0';break;
		case '3': myMap2[j] = '0'; myMap2[j+1] = '0'; myMap2[j+2] = '1'; myMap2[j+3] = '1';break;
		case '4': myMap2[j] = '0'; myMap2[j+1] = '1'; myMap2[j+2] = '0'; myMap2[j+3] = '0';break;
		case '5': myMap2[j] = '0'; myMap2[j+1] = '1'; myMap2[j+2] = '0'; myMap2[j+3] = '1';break;
		case '6': myMap2[j] = '0'; myMap2[j+1] = '1'; myMap2[j+2] = '1'; myMap2[j+3] = '0';break;
		case '7': myMap2[j] = '0'; myMap2[j+1] = '1'; myMap2[j+2] = '1'; myMap2[j+3] = '1';break;
		case '8': myMap2[j] = '1'; myMap2[j+1] = '0'; myMap2[j+2] = '0'; myMap2[j+3] = '0';break;
		case '9': myMap2[j] = '0'; myMap2[j+1] = '0'; myMap2[j+2] = '0'; myMap2[j+3] = '1';break;
		case 'A': myMap2[j] = '1'; myMap2[j+1] = '0'; myMap2[j+2] = '1'; myMap2[j+3] = '0';break;
		case 'B': myMap2[j] = '1'; myMap2[j+1] = '0'; myMap2[j+2] = '1'; myMap2[j+3] = '1';break;
		case 'C': myMap2[j] = '1'; myMap2[j+1] = '1'; myMap2[j+2] = '0'; myMap2[j+3] = '0';break;
		case 'D': myMap2[j] = '1'; myMap2[j+1] = '1'; myMap2[j+2] = '0'; myMap2[j+3] = '1';break;
		case 'E': myMap2[j] = '1'; myMap2[j+1] = '1'; myMap2[j+2] = '1'; myMap2[j+3] = '0';break;
		case 'F': myMap2[j] = '1'; myMap2[j+1] = '1'; myMap2[j+2] = '1'; myMap2[j+3] = '1';break;    
		}   
		j = j + 4;
	}
} 

bool Iso8583::validaISO(void)
{
	int i, elles, tam = 0;
	unsigned int soma = 20;
	char aux[3] = "";
	for ( i = 0 ; i < TAM_FORMAT_ISO ; i++ ) 
	{
		if( myMap[defaultISO[i].bit] == '0'){} 
		else
		{					
			if(defaultISO[i].type == TB_FIX)
			{
				tam = defaultISO[i].lenght;
				elles = 0;
				if( confereDigito(soma, soma+tam, defaultISO[i].bna) == false) return false;
			}else if(defaultISO[i].type == TB_LL)
			{
				elles = 2;
				strncpy (aux, myISO+soma, 2);
				tam = atoi(aux) ;
				if( confereDigito(soma+elles, soma+tam, defaultISO[i].bna) == false) return false;
			}else if(defaultISO[i].type == TB_LLL)
			{
				elles = 3;
				strncpy (aux, myISO+soma, 3);						
				tam = atoi(aux) ;
				if(confereDigito(soma+elles, soma+tam, defaultISO[i].bna) == false) return false;
			}					
			soma = soma + tam + elles;	
		}
	}

	if(soma != strlen(myISO)) return false;

	return true;
}

bool Iso8583::confereDigito(int ini, int fim, char tp)
{
	int i;

	for ( i = ini ; i <= fim ; i++ )
	{
		if(tp == 'N' && isdigit(myISO[i]) == 0)return false;
		if(tp == 'A' && isalnum(myISO[i]) == 0 && !(myISO[i] == '#'))return false;
	}
	return true;
}