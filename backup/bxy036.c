#include<pbc.h>
#include<pbc_test.h>
#include<string.h>
int main()
{
	double start,finish;
	start=pbc_get_time();
	/*******************
	 * Setup
	 *****************/
	pairing_t pairing;
	char param[2048];
	size_t count = fread(param,1,2048,stdin);
	if (!count) pbc_die("input error");
	pairing_init_set_buf(pairing,param,count);
	//define the parameters
	element_t g,f,h,U,J,F,H,temp1,temp2,left,right1,right2,right3,right12,right;
	element_init_G1(g,pairing);
	element_init_G1(f,pairing);
	element_init_G1(h,pairing);
	element_init_G1(U,pairing);
	element_init_G1(J,pairing);
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
	// v
	element_t v[n+1];
	for(int i=0;i<=n;i++){
		element_init_G1(v[i],pairing);
		element_random(v[i]);
	}
	// F
	element_set1(F);
	for(int i=1;i<=n;i++){
		element_pow_mpz(v[i],v[i],m[i-1]);
	}
	for(int i=0;i<=n;i++)
	{
		element_mul(F,F,v[i]);
	}
	element_printf("F is %B\n",F);
	/* long      I wanna F%(2^n),but F is special type.
	mpz_t N;
	mpz_init(N);
	mpz_set_ui(N,2);
	for(int i=1;i<=(n-1);i++){
		mpz_pow_ui(N,N,2);   	// to let N=2^n
	}
	element_t F_n;
	element_init_Zr(F_n);
	element_set_mpz(F_n,N); 	// F_n = 2^n
	for(;;){
		if() 				//I can't write judge condition
		element_sub(F,F,F_n);		// F = F-2^n
	}
	*/
	//C
	char C[]="abcdefg";			//default C="abcdefg"
	int l;
	l=sizeof(C)-1;				// because of  sizeof(string) 
	mpz_t c[l];				// use ASCII code
	for(int i=0;i<n;i++){
		mpz_init_set_ui(c[i],(long)C[i]);	//convert char to integer
	}
	element_t d[l+1];
	element_t u[l+1];
	for(int i=0;i<=l;i++)
	{
		element_init_G1(d[i],pairing);
		element_random(d[i]);
		element_init_G1(u[i],pairing);
		element_random(u[i]);
	}
	// U
	element_set1(U);
	for(int i=1;i<=l;i++){
		element_pow_mpz(d[i],d[i],c[i-1]);
	}
	for(int i=0;i<=l;i++)
	{
		element_mul(U,U,d[i]);
	}
	element_printf("U is %B\n",U);
	// J
	element_set1(J);
	for(int i=1;i<=l;i++){
		element_pow_mpz(u[i],u[i],c[i-1]);
	}
	for(int i=0;i<=l;i++)
	{
		element_mul(J,J,u[i]);
	}
	element_printf("J is %B\n",J);

	//initialize the parameters
	element_random(g);
	element_random(f);
	element_random(h);
	
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

	//initial J and F
//	element_printf("former u[1] is %B\n",u[1]);
//	element_printf("c[1] is %B\n",c[1]);
//	element_pow_zn(u[1],u[1],c[1]);
//	element_printf("%B",u[1]);

	/********************
	 * KeyGen
	 *****************/
	element_t sk_A,sk_B,pk_A,pk_B;
	element_init_Zr(sk_A,pairing);
	element_init_Zr(sk_B,pairing);
	element_init_G1(pk_A,pairing);
	element_init_G1(pk_B,pairing);
	
	element_random(sk_A);			//skA
	element_pow_zn(pk_A,g,sk_A);		//pkA
	/* test if the Pk pair equal 
	element_pairing(temp1,g,g);
	element_pairing(temp2,pk_A1,pk_A2);
	element_printf("temp1 is %B\n",temp1);
	element_printf("temp2 is %B\n",temp2);
	*/
	element_random(sk_B);			//skB
	element_pow_zn(pk_B,g,sk_B);		//pkB
	
	/********************
	 *  ReKeyGen
	 *  *******************/
	element_t rho;
	element_init_Zr(rho,pairing);
	element_random(rho);
	
	char rk0[l+1];
	element_t rk1,rk2;
	element_init_G1(rk1,pairing);
	element_init_G1(rk2,pairing);
	element_t temp31,temp32,temp33,temp34,temp35;
	element_init_Zr(temp31,pairing);
	element_init_G1(temp32,pairing);
	element_init_G1(temp33,pairing);
	element_init_G1(temp34,pairing);
	element_init_G1(temp35,pairing);
	strcpy(rk0,C);				//rk0

	element_invert(temp31,sk_B);		// 1/skB
	element_pow_zn(temp32,U,rho);		// U^rho
	element_mul(temp33,f,temp32);		// f*U^rho
	element_mul(temp34,pk_A,temp33);	// pkA*f*U^rho
	element_pow_zn(rk1,temp34,temp31);	//rk1  =(pkA*f*U^rho)^1/skB

	element_pow_zn(rk2,g,rho);		//rk2  =g^rho
	/**********************
	 * ReSigKeyVer
	 * *****************/
	printf("rk0 = c\t");
	for(int i=0;i<=l;i++){
		if(rk0[i]==C[i]){
		//	continue;
		}else{
			printf("no\n");
			break;
		}
		if(i==l){
			printf("yes!\n");
		}
	}

	printf("rk2 != 1 \t");
	if(element_is1(rk2)!=1){
		printf("yes!\n");
	}else{
		printf("no\n");
	}
	
	printf("verify the ReKey \n");
	element_pairing(left,rk1,pk_B);
	element_printf("left : %B\n",left);
	element_pairing(right1,g,pk_A);
	element_pairing(right2,f,g);
	element_pairing(right3,U,rk2);
	element_mul(right12,right1,right2);
	element_mul(right,right12,right3);
	if(!element_cmp(left,right)){
		printf("\tThe ReKey's verification passed!\n");
	}else{
		printf("verification failed!!\n");
	}
	/****************
	 * Sign
	 * ***************/
	element_t s,r;
	element_t sig0,sig1,sig2;
	element_init_G1(sig0,pairing);
	element_init_G1(sig1,pairing);
	element_init_G1(sig2,pairing);
	element_init_Zr(s,pairing);	
	element_init_Zr(r,pairing);	
	element_random(s);
	element_random(r);
	element_t temp41,temp42,temp43,temp44;
	element_init_G1(temp41,pairing);
	element_init_G1(temp42,pairing);
	element_init_G1(temp43,pairing);
	element_init_G1(temp44,pairing);
	element_pow_zn(temp41,F,s);		//F^s
	element_pow_zn(temp42,J,r);		//J^r
	element_pow_zn(temp43,h,sk_A);		//h^skA
	element_mul(temp44,temp41,temp42);	//J^r*F^s
	element_mul(sig0,temp43,temp44);	//sig0  =h^sk_a*J^r*F^s

	element_pow_zn(sig1,g,r);		//sig1  =g^r

	element_pow_zn(sig2,g,s);		//sig2  =g^s	
	
	/**************
	 * Re-Signing
	 * ************/
	printf("\nVerify the first-level signature :\n");
	if(element_is1(sig1)!=1){
		printf("sig1 != 1\n");
	}else{
		printf("fail!\n");
	}
	if(element_is1(sig2)!=1){
		printf("sig2 != 1\n");
	}else{
		printf("fail!\n");
	}
	element_pairing(left,sig0,g);
	element_printf("left : %B\n",left);
	element_pairing(right1,h,pk_A);
	element_pairing(right2,J,sig1);
	element_pairing(right3,F,sig2);
	element_mul(right12,right1,right2);
	element_mul(right,right12,right3);
	/*
//	element_printf("right1: %B\n",right1);
//	element_printf("right2: %B\n",right2);
//	element_printf("right3: %B\n",right3);
	element_printf("right : %B\n",right);
	*/
	if(!element_cmp(left,right)){
		printf("The first-level signature's verification passed!\n");
	}else{
		printf("verification failed!!\n");
	}
	element_t resig0,resig1,resig2,resig3,resig4,resig5,resig6;
	element_t t,r_,s_;
	element_init_G1(resig0,pairing);
	element_init_G1(resig1,pairing);
	element_init_G1(resig2,pairing);
	element_init_G1(resig3,pairing);
	element_init_G1(resig4,pairing);
	element_init_G1(resig5,pairing);
	element_init_G1(resig6,pairing);
	element_init_Zr(t,pairing);
	element_init_Zr(r_,pairing);
	element_init_Zr(s_,pairing);
	element_random(t);
	element_random(r_);
	element_random(s_);

	element_t temp51,temp52,temp53,temp54,temp55,temp56,temp57,temp58;
	element_init_G1(temp51,pairing);
	element_init_G1(temp52,pairing);
	element_init_G1(temp53,pairing);
	element_init_G1(temp54,pairing);
	element_init_G1(temp55,pairing);
	element_init_G1(temp56,pairing);
	element_init_G1(temp57,pairing);
	element_init_G1(temp58,pairing);
	
	element_pow_zn(temp51,sig0,t);		//sig0^t
	element_pow_zn(temp52,J,r_);		//J^r_
	element_pow_zn(temp53,F,s_);		//F^s_
	element_mul(temp54,temp53,temp52);	//J^r_*F^s_
	element_mul(resig0,temp51,temp54);	//resig0  =sig0^t*J^r_*F^s_
	
	element_pow_zn(temp55,sig1,t);		//sig1^t
	element_pow_zn(temp56,g,r_);		//g^r_
	element_mul(resig1,temp55,temp56);	//resig1

	element_pow_zn(temp57,sig2,t);		//sig2^t
	element_pow_zn(temp58,g,s_);		//g^s_
	element_mul(resig2,temp58,temp57);	//resig2

	element_pow_zn(resig3,pk_A,t);		//resig3

	element_pow_zn(resig4,rk1,t);		//resig4

	element_pow_zn(resig5,rk2,t);		//resig5

	element_pow_zn(resig6,g,t);		//resig6

	/*****************
	 * Verify
	 * **************/
	if(element_is1(resig1)!=1){
		printf("resig1 != 1\n");
	}else{
		printf("fail!\n");
	}
	if(element_is1(resig2)!=1){
		printf("resig2 != 1\n");
	}else{
		printf("fail!\n");
	}
	printf("\nVerify the second-level signature :\n");
	printf("formula 1 :\n");
	element_pairing(left,resig0,g);
	element_printf("left : %B\n",left);
	element_pairing(right1,h,resig3);
	element_pairing(right2,J,resig1);
	element_pairing(right3,F,resig2);
	element_mul(right12,right1,right2);
	element_mul(right,right12,right3);
	/*
	/////////////
	element_t right23;
	element_init_G1(right23,pairing);
	element_mul(right23,right2,right3);
	element_printf("///////nright2*right3:%B\n///////////\n",right23);
	//////////
	*/
	element_printf("right : %B\n",right);
	if(!element_cmp(left,right)){
		printf("The formula 1  passed!\n");
	}else{
		printf("verification failed!!\n");
	}
	printf("formula 2 :\n");
	element_pairing(left,resig4,pk_B);
	element_printf("left : %B\n",left);
	element_pairing(right1,g,resig3);
	element_pairing(right2,f,resig6);
	element_pairing(right3,U,resig5);
	element_mul(right12,right1,right2);
	element_mul(right,right12,right3);
	//element_printf("right1 : %B\n",right1);
	element_printf("right : %B\n",right);
	if(!element_cmp(left,right)){
		printf("The formula 2  passed!\nAll done!\nCongratulations!!!\n");
	}else{
		printf("verification failed!!\n");
	}


	
	finish=pbc_get_time();
	printf("totaltime is %f\n",finish);
	return 0;
}

