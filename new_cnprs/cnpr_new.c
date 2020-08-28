#include<pbc.h>
#include<pbc_test.h>
int main()
{
	double start,finish;
	/*******************
	 * setup
	 *****************/
	pairing_t pairing;
	char param[1024];
	size_t count = fread(param,1,1024,stdin);
	if (!count) pbc_die("input error");
	pairing_init_set_buf(pairing,param,count);
	//define the parameters
	element_t g,g1,f,f0,h0,J,J0,F,H,temp1,temp2,left,right1,right2,right3,right12,right;
	element_init_G1(g,pairing);
	element_init_G1(g1,pairing);
	element_init_G1(f,pairing);
	element_init_G1(f0,pairing);
	element_init_G1(h0,pairing);
	element_init_G1(J,pairing);
	element_init_G1(J0,pairing);
	element_init_G1(F,pairing);
	element_init_G1(H,pairing);
	element_init_GT(temp1,pairing);
	element_init_GT(temp2,pairing);
	element_init_GT(left,pairing);
	element_init_GT(right1,pairing);
	element_init_GT(right2,pairing);
	element_init_GT(right3,pairing);
	element_init_GT(right12,pairing);
	element_init_GT(right,pairing);
	// m 
	FILE *fid;
	fid = fopen("message.txt","rb");
	
	if(fid==0){
		printf("File open error!\n");
		return 0;
	}
					// to get the size of this file
	fseek(fid,0,SEEK_END);		// seek for the end of the file
	int n = (int)ftell(fid);	//return the offset from start to end
	rewind(fid);			// put the point on the start again!
					// read the file
	char pos[n];
	if(pos==NULL){
		printf("error in memoryl");
	}
	fread(pos,sizeof(char),n,fid);
	mpz_t m[n];			// use ASCII code
	for(int i=0;i<n;i++){
		mpz_init_set_ui(m[i],(long)pos[i]);	//convert char to integer
	}
	fclose(fid);
	//v
	element_t v[n];
	for(int i=0;i<n;i++)
	{
		element_init_G1(v[i],pairing);
	}
	//F
	for(int i=0;i<n;i++)
	{
		element_random(v[i]);
	}
	for(int i=1;i<n;i++)
	{
		element_pow_mpz(v[i],v[i],m[i]);
	}
	element_mul(F,v[0],v[1]);
	for(int i=2;i<n;i++)
	{
		element_mul(F,F,v[i]);		//F=v0*[vi^mi]|i:1->n
	}
	element_printf("F is %B\n",F);
	//initialize the parameters
	element_random(g);
	element_random(g1);
	element_random(f);
	element_random(f0);
	element_random(h0);
	element_random(J);
	element_random(J0);
	
	/* testify whether all params are assigned correctly
	element_printf("all params are :%B\n%B\n%B\n%B\n",g,g1,f,h);
	for(int i=0;i<l;i++)
	{
		element_printf("c[i] is %B\n",c[i]);
	}
	for(int i=0;i<n;i++)
	{
		element_printf("m[i] is %B\n",m[i]);
	}
	*/	

	/********************
	 * KeyGen
	 *****************/
	element_t sk_A_inverse,sk_A,sk_B_inverse,sk_B,pk_A1,pk_A2,pk_B1,pk_B2;
	element_init_Zr(sk_A,pairing);
	element_init_Zr(sk_A_inverse,pairing);
	element_init_Zr(sk_B_inverse,pairing);
	element_init_Zr(sk_B,pairing);
	element_init_G1(pk_A1,pairing);
	element_init_G1(pk_A2,pairing);
	element_init_G1(pk_B1,pairing);
	element_init_G1(pk_B2,pairing);
	
	element_random(sk_A);			//skA
	element_pow_zn(pk_A1,g,sk_A);		//pkA1
	element_invert(sk_A_inverse,sk_A);
	element_pow_zn(pk_A2,h0,sk_A_inverse);	//pkA2
	/* test if the Pk pair equal 
	element_pairing(temp1,g,g);
	element_pairing(temp2,pk_A1,pk_A2);
	element_printf("temp1 is %B\n",temp1);
	element_printf("temp2 is %B\n",temp2);
	*/
	element_random(sk_B);			//skB
	element_pow_zn(pk_B1,g,sk_B);		//pkB1
	element_invert(sk_B_inverse,sk_B);
	element_pow_zn(pk_B2,h0,sk_B_inverse);	//pkB2
	
	/********************
	 *  Re-signing KeyGen
	 *  *******************/
	element_t rho;
	char C[]="abcdefg";			//default C="abcdefg"
	element_init_Zr(rho,pairing);
	element_random(rho);
	
	element_t rk1,rk2;
	element_init_G1(rk1,pairing);
	element_init_G1(rk2,pairing);
	element_t temp31,temp32,temp33,temp34,temp35,temp36;
	element_init_G1(temp31,pairing);
	element_init_G1(temp32,pairing);
	element_init_G1(temp33,pairing);
	element_init_G1(temp34,pairing);
	element_init_G1(temp35,pairing);
	element_init_G1(temp36,pairing);
	element_from_hash(H,C,8);		//default H: *->8
	element_pow_zn(temp31,pk_A2,sk_B);	//pkA1^skB
	element_pow_zn(temp32,J0,H);		//J0^H
	element_mul(temp33,f0,temp32);		//f0*J0^H
	element_pow_zn(temp34,temp33,rho);	//(f0*J0^H)^rho
	element_mul(rk1,temp31,temp34);		//rk1 = (pkA2^skB)*((f0*J0^H)^rho)
	
	element_pow_zn(rk2,pk_B2,rho);		//rk2 = pkB2^rho

	/****************
	 * Signing
	 * ***************/
	element_t s0,s1;
	element_t sig0,sig1,sig2;
	element_init_G1(sig0,pairing);
	element_init_G1(sig1,pairing);
	element_init_G1(sig2,pairing);
	element_init_Zr(s0,pairing);	
	element_init_Zr(s1,pairing);	
	element_random(s0);
	element_random(s1);
	element_t temp41,temp42,temp43,temp44,temp45,temp46;
	element_init_G1(temp41,pairing);
	element_init_G1(temp42,pairing);
	element_init_G1(temp43,pairing);
	element_init_G1(temp44,pairing);
	element_init_G1(temp45,pairing);
	element_init_G1(temp46,pairing);
	element_pow_zn(temp41,J,H);		//J^H
	element_mul(temp42,f,temp41);		//f*(J^H)	
	element_pow_zn(temp43,temp42,s0);	//(f*(J^H)^s0

	element_pow_zn(temp44,F,s1);		//F^s1

	element_pow_zn(temp45,g1,sk_A_inverse);		//g1^skA_inverse

	element_mul(sig0,temp43,temp44);	
	element_mul(sig0,sig0,temp45);		//sig0

	element_pow_zn(sig1,h0,s0);		//sig1	

	element_pow_zn(sig2,h0,s1);		//sig2
	
	printf("Verify the first-level signature :\n");
	element_pairing(left,sig0,h0);
	element_printf("left : %B\n",left);
	element_pairing(right1,g1,pk_A2);
	element_pairing(right2,temp42,sig1);
	element_pairing(right3,F,sig2);
	element_mul(right12,right1,right2);
	element_mul(right,right12,right3);
//	element_printf("right1: %B\n",right1);
//	element_printf("right2: %B\n",right2);
//	element_printf("right3: %B\n",right3);
	element_printf("right : %B\n",right);
	if(!element_cmp(left,right)){
		printf("\tThe first-level signature's verification passed!\n");
	}else{
		printf("verification failed!!\n");
	}
	/**************
	 * Re-Signing
	 * ************/
	element_t resig0,resig1,resig2,resig3,resig4,resig5;
	element_t t0,t1,t2;
	element_init_G1(resig0,pairing);
	element_init_G1(resig1,pairing);
	element_init_G1(resig2,pairing);
	element_init_G1(resig3,pairing);
	element_init_G1(resig4,pairing);
	element_init_G1(resig5,pairing);
	element_init_Zr(t0,pairing);
	element_init_Zr(t1,pairing);
	element_init_Zr(t2,pairing);
	element_random(t0);
	element_random(t1);
	element_random(t2);

	element_t temp51,temp52,temp53,temp54,temp55,temp56,temp57;
	element_init_G1(temp51,pairing);
	element_init_G1(temp52,pairing);
	element_init_G1(temp53,pairing);
	element_init_G1(temp54,pairing);
	element_init_G1(temp55,pairing);
	element_init_G1(temp56,pairing);
	element_init_G1(temp57,pairing);
	
	element_pow_zn(temp51,sig0,t0);		//sig0^t0
	element_pow_zn(temp52,temp42,t1);	//(f*(J^H)^t1
	element_pow_zn(temp53,F,t2);		//F^t2
	element_mul(resig0,temp51,temp52);
	element_mul(resig0,resig0,temp53);	//resig0
	
	element_pow_zn(temp54,sig1,t0);		//sig1^t0
	element_pow_zn(temp55,h0,t1);		//h0^t1
	element_mul(resig1,temp54,temp55);	//resig1

	element_pow_zn(temp56,sig2,t0);		//sig2^t0
	element_pow_zn(temp57,h0,t2);		//h0^t2
	element_mul(resig2,temp56,temp57);	//resig2

	element_pow_zn(resig3,pk_A2,t0);	//resig3

	element_pow_zn(resig4,rk1,t0);		//resig4

	element_pow_zn(resig5,rk2,t0);		//resig4

	/*****************
	 * Verify
	 * **************/

	start=pbc_get_time();
	printf("\nVerify the second-level signature :\n");
	printf("formula 1 :\n");
	element_pairing(left,resig0,h0);
	element_printf("left : %B\n",left);
	element_pairing(right1,g1,resig3);
	element_pairing(right2,temp42,resig1);
	element_pairing(right3,F,resig2);
	element_mul(right12,right1,right2);
	element_mul(right,right12,right3);
	/*
	/////////////
	element_t right23;
	element_init_G1(right23,pairing);
	element_mul(right23,right2,right3);
	element_printf("///////\nright2*right3:%B\n///////////\n",right23);
	//////////
	*/
	element_printf("right : %B\n",right);
	if(!element_cmp(left,right)){
		printf("\tThe formula 1  passed!\n");
	}else{
		printf("verification failed!!\n");
	}
	printf("formula 2 :\n");
	element_pairing(left,resig4,pk_B2);
	element_printf("left : %B\n",left);
	element_pairing(right1,h0,resig3);
	element_pairing(right2,temp33,resig5);
	element_mul(right12,right1,right2);
	element_printf("right : %B\n",right12);
	if(!element_cmp(left,right12)){
		printf("\tThe formula 2  passed!\nAll done!\nCongratulations!!!\n");
	}else{
		printf("verification failed!!\n");
	}
	
	finish=pbc_get_time();
	printf("totaltime is %f\n",finish);
	return 0;
}

