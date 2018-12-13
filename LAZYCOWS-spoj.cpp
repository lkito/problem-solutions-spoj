#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//NOT __INT_MAX__ because we might have to add two "infinities", which should still be infinity
#define infinity __INT_MAX__/4;

struct Cow{
    int col;
    int pos; //state of column(where are cows?) 1 = up, 2 = low, 3 = both
};

bool cowComp(const Cow c1, const Cow c2){
    return c1.col < c2.col;
}

int main(int argc, char **argv){
    ios::sync_with_stdio(0);
    int n, k, b, t, colInd, rowInd, result;
    cin >> t;
    for(int test = 0; test < t; test++){
        cin >> n >> k >> b;
        vector<Cow> v;
        for(int i = 0; i < n; i++){
           Cow c;
           cin >> c.pos >> c.col;
           v.push_back(c);
        }
        sort(v.begin(), v.end(), cowComp);
        int dataSize = 1;
        int last = v.at(0).col;
        int cur;
        for(int i = 1; i < n; i++){
            cur = v.at(i).col;
            if(v.at(i).col != last) dataSize++;
            last = cur;
        }
        Cow arr[dataSize];
        int arrIndex = 1;
        arr[0] = v.at(0);
        for(int i = 1; i < n; i++){ //merge cows on same columns
            Cow curCow = v.at(i);
            if(curCow.col == arr[arrIndex - 1].col){ //if there are 2 cows on same column...
                arr[arrIndex - 1].pos = 3;
            } else {
                arr[arrIndex] = curCow;
                arrIndex++;
            }
        }
        //startUp and Down = left most cows on each row, end - right most.
        int startUp, startDown, endUp, endDown, curPos, nextPos;
        bool isSameRow;
        int dist[2][dataSize][dataSize] = {0}; //[0] -- min(one horizontal rect, full rect), [1] -- two horizontal rects
        for(int i = 0; i < dataSize; i++){
            startUp = 0, startDown = 0, endUp = -1, endDown = -1;
            curPos = arr[i].pos;
            if(curPos == 3){ //two cows
                dist[0][i][i] = infinity; //can't do it with 1 hor rec
                dist[1][i][i] = 2; //can do it with 2 hor recs
            } else{
                dist[0][i][i] = 1; //can do it with 1 hor rec
                dist[1][i][i] = infinity; //don't need 2 hor recs
            }
            if(curPos == 2 || curPos == 3){ //2 or 3 means there is a cow on upper row
                startDown = arr[i].col;
                endDown = arr[i].col;
            }
            if(curPos == 1 || curPos == 3){ //there is a cow on lower row
                startUp = arr[i].col;
                endUp = arr[i].col;
            }
            isSameRow = (curPos != 3);
            for(int j = i + 1; j < dataSize; j++) {
                nextPos = arr[j].pos; //state of next col
                isSameRow = (isSameRow && (nextPos == curPos)); //check if these two have the same states
                if(isSameRow){ //if they are same row, we can place them both in a signle hor rect
                    dist[0][i][j] = arr[j].col - arr[i].col + 1;
                } else dist[0][i][j] = 2 * (arr[j].col - arr[i].col) + 2; //cost of building 2xn from start col to this col
                if(startUp == 0 && nextPos != 2){ //if we don't have first cow on upper row yet, memorize this col
                    startUp = arr[j].col;
                    endUp = arr[j].col;
                }
                if(startDown == 0 && nextPos != 1){ //if we don't have first cow on lower row yet, memorize this col
                    startDown = arr[j].col;
                    endDown = arr[j].col;
                }
                if(nextPos != 2) endUp = arr[j].col; //if cow is present, memorize last cols
                if(nextPos != 1) endDown = arr[j].col;
                if(isSameRow){
                    dist[1][i][j] = infinity;
                } else { //if we need both rows, we memorize the cost(this might be same as dist[2][i][j])
                    dist[1][i][j] = endUp - startUp + endDown - startDown + 2;
                }
            }
        }
        int resultArr[k + 1][dataSize]; //[number of rectangles][number of columns]
        for(int i = 0; i < k + 1; i++){
            for(int j = 0; j < dataSize; j++){
                resultArr[i][j] = infinity;
            }
        }
        for(int i = 0; i < dataSize; i++){ //shortest way from first col to every other, might be 2xn or 1xn
            resultArr[1][i] = dist[0][0][i];
        }//because of this cycle program works when k = 1
        int lastResult1, lastResult2;
        for(int i = 2; i <= k; i++) {
            for(int j = 0; j < dataSize; j++) {
                resultArr[i][j] = infinity;
                for(int l = 0; l <= j; l++) {
                    if(l == 0){
                        lastResult1 = 0; //first iter, no preceeding results
                        lastResult2 = 0;
                    } else {
                        lastResult1 = resultArr[i - 1][l - 1]; //results of last iter
                        lastResult2 = resultArr[i - 2][l - 1]; //i - 2 because in this case we are adding 2 rects
                    }
                    resultArr[i][j] = min(resultArr[i][j] , lastResult1 + dist[0][l][j]);
                    resultArr[i][j] = min(resultArr[i][j] , lastResult2 + dist[1][l][j]);
                }
            }
        }
        result = resultArr[k][dataSize - 1];
        cout << result << endl;
    }
    return 0;
}
