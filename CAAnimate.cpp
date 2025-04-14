#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
emp::web::Document doc{"target"};

class CAAnimator : public emp::web::Animate {

// grid width and height
const int num_h_boxes = 20;
const int num_w_boxes = 30;
const double RECT_SIDE = 15;
const double width{num_w_boxes * RECT_SIDE};
const double height{num_h_boxes * RECT_SIDE};


//some vectors to hold information about the CA
std::vector<std::vector<float> > cells;
//vectors to hold the next state of the cells
std::vector<std::vector<float> > nextCells;
//random number generator
emp::Random random;


// where we'll draw
emp::web::Canvas canvas{width, height, "canvas"};

public:

    CAAnimator() {
        // shove canvas into the div
        // along with some control buttons
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        //fill the Cell holders with 0s
        cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));
        nextCells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));
       
        //Initialize cells conditions to random values between 0 and 1
        for (int x = 0; x < num_w_boxes; x++){
            for (int y = 0; y < num_h_boxes; y++) {
                cells[x][y] = random.GetDouble(0,1); 
            }
        }
    }

    void DoFrame() override {
        canvas.Clear();
       
         //draw the grid based on the current state of the cells
         for (int x = 0; x < num_w_boxes; x++){
            for (int y = 0; y < num_h_boxes; y++) {
              float value = cells[x][y];
              canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, emp::ColorHSV(0, 0, value), "black");
           }
       }

        //determine the next state of the every cells
        for (int x = 0; x < num_w_boxes; x++){
            for (int y = 0; y < num_h_boxes; y++) {
                //handles neighbors cell information (with proper wrapping)
                float sum = 0;
                int num_neighbors_alive = 0;
                for (int nx = -1; nx <= 1; nx++){
                    for (int ny = -1; ny <= 1; ny++){
                        if (nx == 0 && ny == 0) continue; // skip the center cell
                        int modx = emp::Mod(x + nx, num_w_boxes);
                        int mody = emp::Mod(y + ny, num_h_boxes);
                        //check number of neighbors alive
                        if (cells[modx][mody] >= 0.5) {
                            num_neighbors_alive++;
                        }
                        //find sum of neighboring cells      
                        sum += cells[modx][mody];
                        }
                    } 
                     /* 
                     Apply rules of CA on current cells and updating status for next generations
                     */
                     float avg = sum/8;
                     if (num_neighbors_alive <= 3) {
                        nextCells[x][y] = avg + (cells[x][y] * 0.7);
                     }
                     if (num_neighbors_alive >= 3) {
                        nextCells[x][y] = avg - (cells[x][y] * 0.7);
                    }
                    
                    //check for out of bounds
                    if (nextCells[x][y] > 1) {
                        nextCells[x][y] = 1;
                    }
                    else if (nextCells[x][y] < 0) {
                        nextCells[x][y] = 0;
                    }
            }
        }
    
        //update the cells to the next state
       cells = nextCells;
}
};

CAAnimator animator;

int main() {
    //Have animator call DoFrame once to start
    animator.Step();
}