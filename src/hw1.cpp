#include "hw1.h"
#include<iomanip>
#include<random>
#include<iostream>
using namespace std;

namespace algebra{

Matrix zeros(size_t n, size_t m){
    Matrix result(n,vector<double>(m,0));
    return result;
}

Matrix ones(size_t n,size_t m){
    Matrix result(n,vector<double>(m,1));
    return result;
}

Matrix random(size_t n,size_t m,double min,double max){
    if(min>max){
        throw std::logic_error("the min is larger than max.");
    }
    default_random_engine e;
    uniform_real_distribution<double> u(min, max);
    Matrix result{zeros(n,m)};
    for(size_t row=0;row<n;row++){
        for(size_t col=0;col<m;col++){
            result[row][col]=u(e);
        }
    }
    return result;
}

void show(const Matrix& matrix){
    if(!matrix.size()){
        return ;
    }
    cout<<setiosflags(ios::fixed)<<setprecision(3);
    size_t ROW=matrix.size(),COL=matrix[0].size();
    for(size_t row=0;row<ROW;row++){
        for(size_t col=0;col<COL;col++){
            cout<<matrix[row][col]<<" ";
        }
        cout<<endl;
    }
}

Matrix multiply(const Matrix& matrix,double c){
    Matrix result=matrix;
    for(auto &vec:result){
        for(auto &num:vec){
            num*=c;
        }
    }
    return result;
}

Matrix multiply(const Matrix& matrix1, const Matrix& matrix2){
    if(!matrix1.size()||!matrix2.size()){
        return Matrix{};
    }
    if(matrix1[0].size()!=matrix2.size()){
        throw std::logic_error("matrices with wrong dimensions cannot be multiplied");
    }
    size_t _row=matrix1.size(),_col=matrix2[0].size();
    size_t _tmp=matrix2.size();
    Matrix answer{zeros(_row,_col)};
    for(size_t row=0;row<_row;row++){
        for(size_t col=0;col<_col;col++){
            for(size_t tmp=0;tmp<_tmp;tmp++){
                answer[row][col]+=matrix1[row][tmp]*matrix2[tmp][col];
            }
        }    
    }
    return answer;
}

Matrix sum(const Matrix& matrix, double c){
    Matrix answer=matrix;
    for(auto &vec:answer){
        for(auto &num:vec){
            num+=c;
        }
    }
    return answer;
}

Matrix sum(const Matrix& matrix1, const Matrix& matrix2){
    if(!matrix1.size()&&!matrix2.size()){
        return Matrix{};
    }
    if(matrix1.size()!=matrix2.size()||matrix1[0].size()!=matrix2[0].size()){
        throw std::logic_error("matrices with wrong dimensions cannot be multiplied");
    }
    size_t _row=matrix1.size(),_col=matrix1[0].size();
    Matrix answer{zeros(_row,_col)};
    for(size_t row=0;row<_row;row++){
        for(size_t col=0;col<_col;col++){
            answer[row][col]=matrix1[row][col]+matrix2[row][col];
        }    
    }
    return answer;
}

Matrix transpose(const Matrix& matrix){
    if(!matrix.size()){
        return Matrix{};
    }
    size_t _row=matrix.size(),_col=matrix[0].size();
    Matrix answer{zeros(_col,_row)};
    for(size_t row=0;row<_row;row++){
        for(size_t col=0;col<_col;col++){
            answer[col][row]=matrix[row][col];
        }
    }        
    return answer;
}

Matrix minor(const Matrix& matrix, size_t n, size_t m){
    size_t _row=matrix.size(),_col=matrix[0].size();
    Matrix answer{zeros(_col-1,_row-1)};
    for(size_t row=0;row<_row;row++){
        for(size_t col=0;col<_col;col++){
            if(row==n||col==m){
                continue;
            }
            answer[row>n?(row-1):row][col>m?(col-1):col]=matrix[row][col];
        }        
    }
    return answer;
}

double determinant(const Matrix& matrix){
    if(!matrix.size()){
        return 1.0;
    }
    if(matrix.size()!=matrix[0].size()){
        throw std::logic_error("non-square matrices have no determinant");
    }
    size_t n=matrix.size();
    if(n==1){
        return matrix[0][0];
    }
    if(n==2){
        return matrix[0][0]*matrix[1][1]-matrix[1][0]*matrix[0][1];
    }
    vector<Matrix>  M;
    for(size_t col=0;col<n;col++){
        M.push_back(minor(matrix,0,col));
    }
    double answer=0;
    for(size_t col=0;col<n;col++){
        answer+=matrix[0][col]*determinant(M[col])*(col%2?(-1):1);
    }
    return answer;
}

Matrix inverse(const Matrix& matrix){
    if(!matrix.size()){
        return Matrix{};
    }
    if(matrix.size()!=matrix[0].size()){
        throw std::logic_error("non-square matrices have no inverse");
    }
    if(determinant(matrix)==0){
        throw std::logic_error("singular matrices have no inverse");
    }
    size_t n=matrix.size();
    Matrix adj_A{zeros(n,n)};
    for(size_t row=0;row<n;row++){
        for(size_t col=0;col<n;col++){
            adj_A[row][col]=((row+col)%2?(-1):1)*determinant(minor(matrix,col,row));
        }
    }
    return multiply(adj_A,1.0/determinant(matrix));
}

Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis=0){
    
}

}