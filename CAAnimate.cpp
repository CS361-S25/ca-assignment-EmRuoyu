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
                float sum = 0;
                int num_alive = 0;

                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = emp::Mod(x + dx, num_w_boxes);
                        int ny = emp::Mod(y + dy, num_h_boxes);
                        float neighbor_val = cells[nx][ny];
                        sum += neighbor_val;
                        if (neighbor_val >= 0.5) num_alive++;
                    }
                }
                float avg = sum / 8.0;
                float current = cells[x][y];

                //CA rule: dynamic decay/growth based on neighborhood density
                float next = current + (avg - current) * 0.7;  
                if (num_alive < 3) next -= 0.8;
                else if (num_alive > 5) next += 0.8;


                // Clamp between 0 and 1
                nextCells[x][y] = std::min(1.0f, std::max(0.0f, next));
            }
        }
    }

    /* Updates the cells to the next state */
    void update_cells() {
        cells = nextCells;
    }
};
CAAnimator animator;

int main() {
    animator.Step();
}
