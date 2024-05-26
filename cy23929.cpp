#include <iostream>
#include <stdio.h>
#include <cmath>

class matrix
{
// only allow this class and other classes that inhherit it to modify this section
protected:
    int num_rows;
    int num_cols;
    double *elements;

// the public section here is free for all parts of the code to access
public:
    // construct a matrix using a pointer to its data and its length
    // we already have a pre-constructed 1d array that contains the data
    // only square matrices can use this algorithm
    matrix(double *data, int length) : num_rows(length), num_cols(length)
    {
        // set memory space for elements
        elements = new double[num_rows * num_cols];
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {   
                // copy the numbers from data to elements
                elements[i * num_cols + j] = data[i * num_cols + j];
            }
        }
    }

    // free memory once program finishes
    virtual ~matrix()
    {
        delete[] elements;
    }

    double get_elem(int i, int j) const
    {
        return elements[i * num_cols + j];
    }
};

// Algorithm class inherits constuctor and functions from Matrix class
class Algorithm : public matrix
{
// only allow this class and other classes that inhherit it to modify this section
protected:
    // error range
    double epsilon = 1e-6;
    // declare memory spaces
    double *alpha_old;
    double *beta_old;
    double *alpha_new;
    double *beta_new;
    // num rows = num cols = n 
    int length;

// This section has functions only meant to be used for the algorithms class 
// It is not meant to be accessed by other classes that inherit from Algorithms or anywhere else
private:
    // Function to compute the norm of the result array
    double find_norm(double *vec)
    {
        double sum = 0;
        for (int i = 0; i < length; i++)
        {
            sum += vec[i] * vec[i]; // Sum of squares of elements
        }
        return sqrt(sum); // Square root of the sum of squares
    }

    // Function to normalise each element in a vector
    void normalize(double *vec)
    {
        double vecNorm = find_norm(vec); // Call function to find norm
        for (int i = 0; i < length; i++)
        {
            vec[i] /= vecNorm; // Normalise every single element in the vector with its norm
        }
    }

    // remark: when get_elem is referred to, this is the format get_elem(rows, cols)
    // normalise and replace element in column of matrix data
    void matrix_mult_alpha()
    {
        for (int i = 0; i < length; i++)
        {
            alpha_new[i] = 0; // Initialize each element of the result vector to 0 for calculations
            for (int j = 0; j < length; j++)
            {
                // we're looping through each element in a column
                // get_elem(rows, cols), the rows change, while the column stays the same
                // this means that they're only doing calculations on just one column
                alpha_new[i] += get_elem(j, i) * beta_old[j];
            }
        }
    }

    // normalise and replace element in row of matrix data
    void matrix_mult_beta()
    {
        for (int i = 0; i < length; i++)
        {
            beta_new[i] = 0; // Initialize each element of the result vector to 0 for calculations
            for (int j = 0; j < length; j++)
            {
                // vice versa for beta
                beta_new[i] += get_elem(i, j) * alpha_new[j];
            }
        }
    }

    // In order to check convergence, take the new vector, old vector and epsilon error range
    // Could be either alpha or beta depending on parameters
    // Each element in the old and new vector is compared to see if its bigger than epsilon or not
    bool check_convergence(double *new_vec, double *old_vec)
    {
        double max_diff = 0.0;
        for (int i = 0; i < length; i++)
        {
            double diff = std::abs(new_vec[i] - old_vec[i]);
            if (diff > max_diff)
            {
                max_diff = diff;
            }
        }
        return max_diff <= epsilon;
    }

// the public section here is free for all parts of the code to access
// this section allows us to construct an algorithm object and call its functions
// which are run and print_scores
public:
    // Constructor of Algorithm class
    Algorithm(double *data, int length) : matrix(data, length), length(length)
    {
        alpha_old = new double[length];
        beta_old = new double[length];
        alpha_new = new double[length];
        beta_new = new double[length];
        for (int i = 0; i < length; i++)
        {
            alpha_new[i] = get_elem(0, i);
            beta_new[i] = get_elem(i, 0);
        }
    }

    // free memory
    ~Algorithm()
    {
        delete[] alpha_old; 
        delete[] beta_old;
        delete[] alpha_new;
        delete[] beta_new;
    }

    void run()
    {
        // Initlialise converged
        // count number of iterations it took to converge
        int iter = 0;
        bool converged = false;

        // Before you run a normalisation in each row/col, the previous new vec will now be the old vec
        // then you find the new alphas/betas with the old alphas/betas
        for (int i = 0; i < length; i++)
        {
            alpha_old[i] = alpha_new[i];
            beta_old[i] = beta_new[i];
        }

        // Perform matrix multiplication and normalization
        matrix_mult_alpha();
        matrix_mult_beta();
        normalize(alpha_new);
        normalize(beta_new);

        iter++;

        // Check convergence for both alpha and beta vectors
        converged = check_convergence(alpha_new, alpha_old) || check_convergence(beta_new, beta_old);

        while (converged == false)
        {
            iter++;

            for (int i = 0; i < length; i++)
            {
                alpha_old[i] = alpha_new[i];
                beta_old[i] = beta_new[i];
            }

            // Perform matrix multiplication and normalization again
            matrix_mult_alpha();
            normalize(alpha_new);

            matrix_mult_beta();
            normalize(beta_new);

            // Check convergence for both alpha and beta vectors again
            converged = check_convergence(alpha_new, alpha_old) || check_convergence(beta_new, beta_old);
        };
        std::cout << "Converges at iteration: " << iter << "\n";
    }

    void print_scores()
    {
        // Print out scores and index
        std::cout << "The Impact score: \n";
        for (int i = 0; i < length; i++)
        {
            std::cout << i + 1 << ": " << alpha_new[i] << "\n";
            // printf("%d : %.2f \n", i + 1, alpha_new[i]);
        }

        std::cout << "The knowledge-base score: \n";
        for (int i = 0; i < length; i++)
        {
            std::cout << i + 1 << ": " << beta_new[i] << "\n";
            // printf("%d : %.2f \n", i + 1, beta_new[i]);
        }
    }
};

int main()
{
    // initialise length and data array
    int length = 7;
    double data[] = {
        0, 1, 1, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 1, 1, 0,
        1, 1, 1, 0, 0, 1, 0,
        1, 0, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 0};

    // create class
    Algorithm hw(data, length);

    // perform the algorithm and print out scores
    hw.run();
    hw.print_scores();

    return 0;
}