#pragma acc parallel loop   /* acc_jacobi1.c   */
    for (i = 1; i <= ROWS; i++) 
        for (j = 1; j <= COLUMNS; j++) {
            Anew[i][j] = 0.25 * (A[i+1][j] + 
            A[i-1][j] + A[i][j+1] + A[i][j-1]);
        }
    dt = 0.0;
        
#pragma acc parallel loop reduction(max:dt)
    for (i = 1; i <= ROWS; i++) 
        for (j = 1; j <= COLUMNS; j++) {
            dt = fmax( fabs(Anew[i][j]-A[i][j]), dt);
            A[i][j] = Anew[i][j];
        }