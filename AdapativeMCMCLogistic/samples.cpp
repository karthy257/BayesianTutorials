// [[Rcpp::depends(RcppArmadillo)]]
#include <RcppArmadillo.h>
using namespace Rcpp;

arma::vec invlogit(arma::vec x){
  arma::vec y = exp(x)/(1 + exp(x));
  return(y);
}

// [[Rcpp::export]]
double berntest(double y, double n, double p, int lg ){
  return R::dbinom(y, n, p, lg);
}

// [[Rcpp::export]]
double log_post(arma::vec b, arma::vec Y, arma::mat X) {
  int n = X.n_rows;
  int p = X.n_cols;
  arma::vec xb;
  arma::vec pi;
  double lik;
  double pr;
  double log_posterior;
  
  xb =  X*b;
  
  for( int i=0; i<n; i++){
    if( xb[i] > 10 ){
      xb[i] = 10;
    }else if( xb[i] < -10 ){
      xb[i] = -10;
    }else{
      xb[i] = xb[i];
    }
  } 
  
  pi = invlogit(xb);
  
  lik = 0;
  for(int i=0; i<n; i++){
    lik = lik + R::dbinom(Y[i], 1, pi[i], 1);
  }
  
  
  pr=0;
  for(int i=0; i<p; i++){
    pr = pr + R::dnorm4(b[i], 0, 1000, 1);
  }
  
  log_posterior = lik + pr;

  return log_posterior;
}

