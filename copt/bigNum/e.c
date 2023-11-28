#include<stdio.h>
#include<math.h>
#include <gmp.h>

// Ce programme test la précision 

double dexpo(double tol){
    double e=1,u=1,k=1;
    while (u>tol) {
        u/=k;
        e+=u;
        k+=1;
    }
    return e;
}

void expo_gmp(mpf_t e,mpf_t u, mpf_t tol){
    size_t k=1;
    while (  mpf_cmp(u,tol) > 0  ) {
        mpf_div_ui(u,u,k);
        mpf_add(e,e,u);
        k++;
    }
}

int main(){
    double dtol=1e-306;
    mpf_t e,u,tol;
    int ndigits=100;
    printf("expo   (double)     : %22.*lf \n",ndigits,dexpo(dtol));
    printf("math.h (double)     : %22.*lf \n",ndigits,exp(1));
    for (size_t nbits=32;nbits<4096;nbits*=2) {
        mpf_init2 (e, nbits);
        mpf_init2 (u, nbits);
        mpf_init2 (tol, nbits);
        mpf_set_ui(e,1);
        mpf_set_ui(u,1);
        mpf_set_d(tol,dtol);
        expo_gmp(e,u,tol);
        gmp_printf("gmp    (%4d bits)  : %22.*Ff\n", nbits,ndigits,e);
    }
    // 2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274274663919320030599218174135966
    printf("table               : 2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274\n");
    printf(" \n");
    mpf_clear(e);
    mpf_clear(u);
    mpf_clear(tol);
}
