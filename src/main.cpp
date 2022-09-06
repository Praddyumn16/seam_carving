#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

void calculate_energy(double** energy, int ***rgb, int H, int W, int C){

    for(int i = 0 ; i < H ; i++){
        
        for(int j = 0 ; j < W ; j++){
            
            int x_gradient = 0 , y_gradient = 0;

            for(int k = 0 ; k < C ; k++){
                
                int x = 0 , y = 0;

                //calculating Rx , Gx , Bx
                if(i == 0)
                    x = rgb[1][j][k] - rgb[H-1][j][k];
                else if(i == H-1)
                    x = rgb[0][j][k] - rgb[H-2][j][k];
                else
                    x = rgb[i-1][j][k] - rgb[i+1][j][k];
                
                //calculating Ry , Gy , By
                if(j == 0)
                    y = rgb[i][1][k] - rgb[i][W-1][k];
                else if(j == W-1)
                    y = rgb[i][0][k] - rgb[i][W-2][k];
                else
                    y = rgb[i][j-1][k] - rgb[i][j+1][k];

                x_gradient += (x * x);
                y_gradient += (y * y);
            }

            energy[i][j] = sqrt(x_gradient + y_gradient);
        }
    }

}

void calculate_cost_vertical(double** cost , int H , int W){

    for(int i = 1 ; i < H ; i++){
        for(int j = 0 ; j < W ; j++){
            if(j == 0)
                cost[i][j] += min(cost[i-1][j] , cost[i-1][j+1]);
            else if(j == W-1)
                cost[i][j] += min(cost[i-1][j-1] , cost[i-1][j]);
            else
                cost[i][j] += min(cost[i-1][j-1] , min(cost[i-1][j] , cost[i-1][j+1]));
        }
    }

}



void calculate_cost_horizontal(double** cost , int H , int W){

    for(int j = W-2 ; j >= 0 ; j--){
        for(int i = 0 ; i < H ; i++){
            if(i == 0)
                cost[i][j] += min(cost[i][j+1] , cost[i+1][j+1]);
            else if(i == H-1)
                cost[i][j] += min(cost[i][j+1] , cost[i-1][j+1]);
            else
                cost[i][j] += min(cost[i-1][j+1] , min(cost[i][j+1] , cost[i+1][j+1]));
        }
    }

}



void solve(int ***rgb, int H, int W, int C, int H_, int W_, int C_) {
    // We've provided you the driver.py and main.cpp for your convinience
    // Please go through them and understand the file handling and input/output format
    // Feel free to experiment on your own

    // can access the array using rgb[i][j][k] like a regular 3D array

    // Write your code here

    while(W > W_){ //removing the vertical seam
    
        double** energy;
        energy = new double *[H];
        for(int i = 0 ; i < H ; i++)
            energy[i] = new double[W];
        
        calculate_energy(energy, rgb , H , W , C);
        
        double** cost = energy;
        calculate_cost_vertical(cost , H , W);
        
        int min_val_idx = 0 , min_val = cost[H-1][0];
        
        for(int i = 0 ; i < W ; i++){
            if(cost[H-1][i] <= min_val){
                min_val = cost[H-1][i];
                min_val_idx = i;
            }
        }

        for(int row = H-1 ; row >= 1 ; row--){
            //shifting the columns by 1 in left
            for(int col = min_val_idx ; col < W - 1 ; col++){
                rgb[row][col][0] = rgb[row][col+1][0];
                rgb[row][col][1] = rgb[row][col+1][1];
                rgb[row][col][2] = rgb[row][col+1][2];
            }

            int col = min_val_idx;

            if(min_val_idx == 0){
                min_val_idx = (cost[row-1][col] < cost[row-1][col+1] ? col : col+1);
            }
            else if(min_val_idx == W-1){
                min_val_idx = (cost[row-1][col-1] < cost[row-1][col] ? col-1 : col);
            }
            else{
                if(cost[row-1][col-1] < cost[row-1][col]){
                    if(cost[row-1][col-1] < cost[row-1][col+1])
                        min_val_idx = col-1;
                    else
                        min_val_idx = col+1;
                }
                else{
                    if(cost[row-1][col] < cost[row-1][col+1])
                        min_val_idx = col;
                    else   
                        min_val_idx = col+1;
                }
            }
        }
        for(int i = 0 ; i < H ; i++){
            delete [] energy[i];
        }
        delete [] energy;
        
        W--;
    }

    while (H > H_){ //removing the horizontal seam

        double** energy;
        energy = new double *[H];
        for(int i = 0 ; i < H ; i++)
            energy[i] = new double[W];
        
        calculate_energy(energy, rgb , H , W , C);

        double** cost = energy;
        calculate_cost_horizontal(energy , H , W);
        
        int min_val_idx = 0 , min_val = cost[0][0];
        
        for(int i = 0 ; i < H ; i++){
            if(cost[i][0] < min_val){
                min_val = cost[i][0];
                min_val_idx = i;
            }
        }

        for(int col = 0 ; col < W-1 ; col++){
            //shifting rows by 1 up
            for(int row = min_val_idx ; row < H-1 ; row++){
                rgb[row][col][0] = rgb[row+1][col][0];
                rgb[row][col][1] = rgb[row+1][col][1];
                rgb[row][col][2] = rgb[row+1][col][2];
            }

            int row = min_val_idx;

            if(min_val_idx == 0){
                min_val_idx = (cost[row][col+1] < cost[row+1][col+1] ? row : row+1);
            }
            else if(min_val_idx == H-1){
                min_val_idx = (cost[row-1][col+1] < cost[row][col+1] ? row-1 : row);
            }
            else{
                if(cost[row-1][col+1] < cost[row][col+1]){
                    if(cost[row-1][col+1] < cost[row+1][col+1])
                        min_val_idx = row-1;
                    else
                        min_val_idx = row+1;
                }
                else{
                    if(cost[row][col+1] < cost[row+1][col+1])
                        min_val_idx = row;
                    else    
                        min_val_idx = row+1;
                }
            }
        }
        for(int i = 0 ; i < H ; i++){
            delete [] energy[i];
        }
        delete [] energy;
       
        H--;
    }
    
}

int main() {
    string ip_dir = "./data/input/";
    string ip_file = "rgb_in.txt";
    ifstream fin(ip_dir + ip_file);

    int H, W, C;
    fin >> H >> W >> C;

    int ***rgb;
    rgb = new int **[H];
    for(int i = 0; i < H; ++i) {
        rgb[i] = new int *[W];
        for(int j = 0; j < W; ++j) {
            rgb[i][j] = new int[C];
            for(int k = 0; k < C; ++k)
                fin >> rgb[i][j][k];
        }
    }
    fin.close();

    int H_, W_, C_;
    cout << "Enter new value for H (must be less than " << H << "): ";
    cin >> H_;
    cout << "Enter new value for W (must be less than " << W << "): ";
    cin >> W_;
    cout << '\n';
    C_ = C;

    solve(rgb, H, W, C, H_, W_, C_);

    string op_dir = "./data/output/";
    string op_file = "rgb_out.txt";
    ofstream fout(op_dir + op_file);
    
    fout << H_ << " " << W_ << " " << C_ << endl;
    for(int i = 0; i < H_; ++i) {
        for(int j = 0; j < W_; ++j) {
            for(int k = 0; k < C_; ++k) {
                fout << rgb[i][j][k] << " ";
            }
        }
        fout << '\n';
    }
    fout.close();

    return 0;
}