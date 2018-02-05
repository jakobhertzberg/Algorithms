nQueens = function(n){
  #Args:
  #     n (integer): value that determines the size of the board
  #Return:
  #     result(matrix): matrix containing 1 and 0 entries where 1 denotes that a queen was placed in the field.
  
  library(glpkAPI)
  
  boardpos<-matrix(1:(n*n),nrow=n,ncol=n,byrow=TRUE) #matrix object of size n*n
  
  #Constraints
  diagonal1<-split(boardpos,row(boardpos)-col(boardpos)) #diagonals going from top left to bottom right
  diagonal2<-split(boardpos,row(boardpos)+col(boardpos)) #diagonals going from bottom let to top right
  ja<-c(split(1:(n*n),1:n),split(c(boardpos),1:n),diagonal1,diagonal2) #column indices for non negative elements of the constraints.
  ia<-c() #1. row constraints 2. column constraints 3. diagonal constraints
  for(i in 1:length(ja)){ #adds row indices for non negative elements of the constraints
    ia<-c(ia,rep(i,length(unlist(ja[i]))))
  }
  ja<-unlist(ja,use.names = FALSE) 
  ar<-rep(1,length(ja)) #coeffiecent of value 1 at all non negative positions
  
  
  
  lp <- initProbGLPK() #initialize lp
  setObjDirGLPK(lp, GLP_MAX) 
  rows<-2*n+((4*n)-2) #2*n since for row and column constraints. 2*n-1 for each of the possible diagonals
  addRowsGLPK(lp,nrows=rows) 
  addColsGLPK(lp,ncols=n*n)
  lbrows <- rep(1,n*2)
  ubrows <-rep(1,rows)
  type <- c(rep(GLP_FX,n*2),rep(GLP_UP,((4*n)-2))) #sets fixed row and column constraints (=1) and diagonal constraints with upper bounds (<=1) 
  setRowsBndsGLPK(lp, 1:rows, lb=lbrows, ub=ubrows, type)
  type<-c(rep(GLP_DB,n*n)) #sets upper (1) and lower (0) bounds for the columns
  lbcols<-rep(0,n*n)
  ubcols<-rep(1,n*n)
  setColsKindGLPK(lp,c(1:(n*n)),rep(GLP_BV,n*n)) #sets kind of columns to binomial
  obj<-rep(1,n*n) #objective function
  setColsBndsObjCoefsGLPK(lp, 1:(n*n), lbcols, ubcols, obj, type)
  loadMatrixGLPK(lp,length(ia), ia, ja, ar) #load constraint matrix
  setMIPParmGLPK(PRESOLVE, GLP_ON) #presolve MIP with simplex
  setMIPParmGLPK(MSG_LEV, GLP_MSG_OFF) #prevents solveMIPGLPK from printing
  solveMIPGLPK(lp) 
  sol<-mipColsValGLPK(lp) #optimal solution
  return(matrix(sol,nrow = n, ncol = n, byrow = TRUE)) #return optimal solution as a matrix
}
