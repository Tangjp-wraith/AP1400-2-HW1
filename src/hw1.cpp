#include "hw1.h"
#include<iomanip>
#include<random>
#include<iostream>
using namespace std;

Matrix algebra::zeros(size_t n, size_t m){
    Matrix result(n,vector<double>(m,0));
    return result;
}

Matrix algebra::ones(size_t n,size_t m){
    Matrix result(n,vector<double>(m,1));
    return result;
}

Matrix algebra::random(size_t n,size_t m,double min,double max){
    if(min>max){
        throw std::logic_error("the min is larger than max.");
    }
    default_random_engine e;
    uniform_real_distribution<double> u(min, max);
    Matrix result{algebra::zeros(n,m)};
    for(size_t row=0;row<n;row++){
        for(size_t col=0;col<m;col++){
            result[row][col]=u(e);
        }
    }
    return result;
}

void algebra::show(const Matrix& matrix){
    cout<<setiosflags(ios::fixed)<<setprecision(3);
    size_t ROW=matrix.size(),COL=matrix[0].size();
    for(size_t row=0;row<ROW;row++){
        for(size_t col=0;col<COL;col++){
            cout<<matrix[row][col]<<" ";
        }
        cout<<endl;
    }
}

Matrix algebra::multiply(const Matrix& matrix,double c){
    Matrix result=matrix;
    for(auto &vec:result){
        for(auto &num:vec){
            num*=c;
        }
    }
    return result;
}

Matrix algebra::multiply(const Matrix& matrix1, const Matrix& matrix2){
    if(matrix1[0].size()!=matrix2.size()){
        throw std::logic_error("matrices with wrong dimensions cannot be multiplied");
    }
    if(!matrix1.size()||!matrix2.size()){
        return Matrix{};
    }
    size_t _row=matrix1.size(),_col=matrix2[0].size();
    size_t _tmp=matrix2.size();
    Matrix answer{algebra::zeros(_row,_col)};
    for(size_t row=0;row<_row;row++){
        for(size_t col=0;col<_col;col++){
            for(size_t tmp=0;tmp<_tmp;tmp++){
                answer[row][col]+=matrix1[row][tmp]*matrix2[tmp][col];
            }
        }    
    }
    return answer;
}