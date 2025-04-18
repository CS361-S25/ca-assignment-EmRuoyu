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
    std::vector<std::vector<float>> cells;
    std::vector<std::vector<float>> nextCells;
    emp::Random random;

    // where we'll draw
    emp::web::Canvas canvas{width, height, "canvas"};

public:

    /* Constructor: initializes canvas, control buttons, and cell grid with random values */
    CAAnimator() {
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));
        nextCells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));

        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                cells[x][y] = random.GetDouble(0, 1);
            }
        }
    }

    /* Called on each frame: draws current state and applies CA update rule */
    void DoFrame() override {
        canvas.Clear();
        draw_cells();
        apply_rules();
        update_cells();
    }

private:

    /* Draws each cell as a grayscale square based on its current value */
    void draw_cells() {
        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                float value = cells[x][y];
                canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, emp::ColorHSV(0, 0, value), "black");
            }
        }
    }

    /* Computes the next state of the grid using custom CA rules */
    void apply_rules() {
        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
        
                int alive_neighbors = count_alive_neighbors(x,y);
                float neighbor_average = average_neighbors(x,y);
                float next_value = 0.0f;

                // Apply CA rules
                if (alive_neighbors < 5 && alive_neighbors > 1) {
                   next_value = neighbor_average + (cells[x][y] * 0.8);
                }
                else if (alive_neighbors > 5 && alive_neighbors < 7) {
                   next_value = neighbor_average - (cells[x][y] * 0.8);
               }

                // Clamp between 0 and 1
                nextCells[x][y] = std::min(1.0f, std::max(0.0f, next_value));
            }
        }
    }

    /* Updates the cells to the next state */
    void update_cells() {
        cells = nextCells;
    }

    /* Count the number of neighbors alive for a given cell */
    int count_alive_neighbors(int x, int y){
        int alive_neighbors = 0;
        for (int horizontal = -1; horizontal <= 1; horizontal++) {
            for (int vertical = -1; vertical <= 1; vertical++) {
                if (horizontal == 0 && vertical == 0) continue; // skip center
                int neighbor_x = emp::Mod(x + horizontal, num_w_boxes);
                int neighbor_y = emp::Mod(y + vertical, num_h_boxes);
                if (cells[neighbor_x][neighbor_y] > 0.5) {
                    alive_neighbors++;
                }
            }
        }
        return alive_neighbors;
    }

    /* Find the average values of the neighbors for a given cell */
    float average_neighbors(int x, int y){
        float sum = 0;
        for (int horizontal = -1; horizontal <= 1; horizontal++) {
            for (int vertical = -1; vertical <= 1; vertical++) {
                if (horizontal == 0 && vertical == 0) continue; // skip center
                int neighbor_x = emp::Mod(x + horizontal, num_w_boxes);
                int neighbor_y = emp::Mod(y + vertical, num_h_boxes);
                sum += cells[neighbor_x][neighbor_y];
            }
        }
        return (sum/8);
    }

};
CAAnimator animator;

int main() {
    animator.Step();
}
