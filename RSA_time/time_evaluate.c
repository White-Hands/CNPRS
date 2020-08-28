#include<stdio.h>
#include<pbc.h>
#include<pbc_test.h>
int main()
{
	pairing_t pairing;
	char param[2048];
	size_t count = fread(param,1,2048,stdin);
	if (!count) pbc_die("input error");
	pairing_init_set_buf(pairing,param,count);

	element_t E,M1,E2;
	element_init_G1(E,pairing);
	element_init_Zr(M1,pairing);
	element_init_G1(E2,pairing);
	
	element_random(E);
	element_random(M1);
	element_random(E2);

	//E
	double e_start,e_end,e_total;
	e_total = 0.0;
	for(int i=1;i<=1000;i++){
		e_start = pbc_get_time();
		element_pow_zn(E,E,M1);
		e_end = pbc_get_time();
		e_total+=(e_end-e_start);
	}	
	printf("E is :%f\n",e_total);

	//M
	double m_start,m_end,m_total;
	m_total=0.0;
	for(int i=1;i<=1000;i++){
	m_start = pbc_get_time();
		element_mul(E2,E,E2);
	m_end = pbc_get_time();
	m_total+=(m_end-m_start);
	}
	printf("M is :%f\n",m_total);

	//P
	double p_start,p_end,p_total;
	p_total=0.0;
	element_t E1,ET;
	element_init_G1(E1,pairing);
	element_init_GT(ET,pairing);
	element_random(E1);
	for(int i=1;i<=1000;i++){
	p_start = pbc_get_time();
		element_pairing(ET,E1,E2);
	p_end = pbc_get_time();
	p_total+=(p_end-p_start);
	}
	printf("P is :%f\n",p_total);


	return 0;
}
