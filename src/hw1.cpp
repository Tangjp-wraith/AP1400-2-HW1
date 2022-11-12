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
    if(!matrix1.size()||!matrix2.size()){
        return Matrix{};
    }
    if(matrix1[0].size()!=matrix2.size()){
        throw std::logic_error("matrices with wrong dimensions cannot be multiplied");
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

Matrix algebra::sum(const Matrix& matrix, double c){
    Matrix answer=matrix;
    for(auto &vec:answer){
        for(auto &num:vec){
            num+=c;
        }
    }
    return answer;
}

Matrix algebra::sum(const Matrix& matrix1, const Matrix& matrix2){
    if(!matrix1.size()&&!matrix2.size()){
        return Matrix{};
    }
    if(matrix1.size()!=matrix2.size()||matrix1[0].size()!=matrix2[0].size()){
        throw std::logic_error("matrices with wrong dimensions cannot be multiplied");
    }
    size_t _row=matrix1.size(),_col=matrix1[0].size();
    Matrix answer{algebra::zeros(_row,_col)};
    for(size_t row=0;row<_row;row++){
        for(size_t col=0;col<_col;col++){
            answer[row][col]=matrix1[row][col]+matrix2[row][col];
        }    
    }
    return answer;
}

Matrix algebra::transpose(const Matrix& matrix){
    if(!matrix.size()){
        return Matrix{};
    }
    size_t _row=matrix.size(),_col=matrix[0].size();
    Matrix answer{algebra::zeros(_col,_row)};
    for(size_t row=0;row<_row;row++){
        for(size_t col=0;col<_col;col++){
            answer[col][row]=matrix[row][col];
        }
    }        
    return answer;
}

Matrix algebra::minor(const Matrix& matrix, size_t n, size_t m){
    size_t _row=matrix.size(),_col=matrix[0].size();
    Matrix answer{algebra::zeros(_col-1,_row-1)};
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

double algebra::determinant(const Matrix& matrix){
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
        M.push_back(algebra::minor(matrix,0,col));
    }
    double answer=0;
    for(size_t col=0;col<n;col++){
        answer+=matrix[0][col]*algebra::determinant(M[col])*(col%2?(-1):1);
    }
    return answer;
}

Matrix algebra::inverse(const Matrix& matrix){
    if(!matrix.size()){
        return Matrix{};
    }
    if(matrix.size()!=matrix[0].size()){
        throw std::logic_error("non-square matrices have no inverse");
    }
    if(algebra::determinant(matrix)==0){
        throw std::logic_error("singular matrices have no inverse");
    }
    size_t n=matrix.size();
    Matrix adj_A{algebra::zeros(n,n)};
    for(size_t row=0;row<n;row++){
        for(size_t col=0;col<n;col++){
            adj_A[row][col]=((row+col)%2?(-1):1)*algebra::determinant(minor(matrix,col,row));
        }
    }
    return algebra::multiply(adj_A,1.0/algebra::determinant(matrix));
}

Matrix algebra::concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis=0){
    if(!matrix1.size()&&!matrix2.size()){
        return Matrix{};
    }
    if((!matrix1.size()&&matrix2.size())||(matrix1.size()&&!matrix2.size())){
        throw std::logic_error("matrices with wrong dimensions cannot be concatenated");
    }
    size_t _row1=matrix1.size(),_col1=matrix1[0].size();
    size_t _row2=matrix2.size(),_col2=matrix2[0].size();
    if(!axis){
        if(_col1!=_col2){
            throw std::logic_error("matrices with wrong dimensions cannot be concatenated");
        }
         Matrix answer{algebra::zeros(_row1+_row2,_col1)};
         for(size_t row=0;row<(_row1+_row2);row++){
            for(size_t col=0;col<_col1;col++){
                answer[row][col]=(row<_row1)?matrix1[row][col]:matrix2[row-_row1][col];
            }
         }
         return answer;
    }else{
        if(_row1!=_row2){
            throw std::logic_error("matrices with wrong dimensions cannot be concatenated");
        }
         Matrix answer{algebra::zeros(_row1,_col1+_col2)};
         for(size_t row=0;row<_row1;row++){
            for(size_t col=0;col<(_col1+_col2);col++){
                answer[row][col]=(col<_col1)?matrix1[row][col]:matrix2[row][col-_col1];
            }
         }
         return answer;
    }
}

Matrix algebra::ero_swap(const Matrix& matrix, size_t r1, size_t r2){
    if(matrix.size()<=r1||matrix.size()<=r2){
        throw std::logic_error("r1 or r2 inputs are out of range");
    }
    Matrix answer=matrix;
    for(size_t col=0;col<matrix[0].size();col++){
        answer[r1][col]=matrix[r2][col];
        answer[r2][col]=matrix[r1][col];
    }
    return answer;
}

Matrix algebra::ero_multiply(const Matrix& matrix, size_t r, double c){
    if(matrix.size()<=r){
        throw std::logic_error("r inputs are out of range");
    }
    Matrix answer=matrix;
    for(size_t col=0;col<matrix[0].size();col++){
        answer[r][col]=matrix[r][col]*c;
    }
    return answer;
}

Matrix algebra::ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2){
    if(matrix.size()<=r1||matrix.size()<=r2){
        throw std::logic_error("r1 or r2 inputs are out of range");
    }
    Matrix answer=matrix;
    for(size_t col=0;col<matrix[0].size();col++){
        answer[r2][col]+=matrix[r1][col]*c;
    }
    return answer;
}

Matrix algebra::upper_triangular(const Matrix& matrix){
    if(matrix.size()<=1){
        return matrix;
    }   
    if(matrix.size()!=matrix[0].size()){
        throw std::logic_error("non-square matrices have no upper triangular form");
    }
    size_t _row=matrix.size();
    Matrix answer=matrix;
    size_t col=0,temp=1;
    for(size_t row=0;row<_row-1;row++){
        while(!answer[row][col]){
            answer=ero_swap(answer,row,temp);
            temp++;
        }
        temp=row+1;
        for(size_t row1=row+1;row1<_row;row1++){
            answer=ero_sum(answer,row,-answer[row1][col]/answer[row][col],row1);
        }
        col++;
    }
    return answer;
}