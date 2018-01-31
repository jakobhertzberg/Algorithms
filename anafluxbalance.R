
calculatemax<-function(S,obj,lb,ub){
  #this function takes the S matrix,objective function and lower as well as upper bounds as input
  #and returns the LP and the corresponding optimal value
  lp <- initProbGLPK() #initialize lp
  setObjDirGLPK(lp, GLP_MAX) 
  rows<-S[[1]]@Dim[1] #initialize rows and columns
  columns<-S[[1]]@Dim[2]
  addRowsGLPK(lp,nrows=rows)
  addColsGLPK(lp,ncols=columns)
  lbrows <- rep(0,rows)
  ubrows <-rep(0,rows)
  type <- rep(GLP_FX,rows)
  setRowsBndsGLPK(lp, 1:rows, lb=lbrows, ub=ubrows, type) #set bounds in order to satisfy Sv=0
  
  #set reaction bounds to the given upper and lower bounds. If both bounds are equal
  #set the type to fixed otherwise to double bounded
  type<-c()
  for(i in 1:length(lb)){
    if(lb[i]==ub[i]){
      type<-c(type,GLP_FX)
    }
    else{
      type<-c(type,GLP_DB)
    }
  }
  setColsBndsObjCoefsGLPK(lp, 1:columns, lb, ub, obj, type)
  
  #loads the rowindex (ia) and column index(ja) for each non zero entry with values (ar) into the constraint matrix.
  summary<-summary(S[[1]])
  ia <- summary$i
  ja <- summary$j
  ar <- summary$x
  loadMatrixGLPK(lp,length(ia), ia, ja, ar)
  
  #solve with simplex algorithm and return LP and optimal value
  solveSimplexGLPK(lp)
  optval<-getObjValGLPK(lp)
  return(list(lp=lp,optval=optval))
}

analyzefluxbal<-function(matfile,Biomass,output){
  #this function returns either a txt file with the given ouput name containing the rates of all reactions
  #(biomass reaction at first place otherwise sorted by reaction index) if Biomass="BIOMASS". Otherwise it returns
  #an array filled with the names of all blocked reactions if Biomass="BLOCKED". The input matfile has to be of the form
  # "example.mat".
  library(R.matlab)
  library(glpkAPI)
  network<-readMat(matfile)$network #read network information of matfile and initialize parameters
  S<-unlist(network[1])
  rev<-unlist(network[2])
  rxns<-unlist(network[3])
  lb<-unlist(network[4])
  ub<-unlist(network[5])
  description<-unlist(network[6])

  if(Biomass=="BIOMASS"){
    mainreaction<-c() 
    for(i in 1:S[[1]]@Dim[2]){ #get the index of the biomass reaction with grepl
      if(grepl("biomass",rxns[i],ignore.case = TRUE)){
        mainreaction<-c(mainreaction,i)
      }
    }

    obj<-rep(0,length(rxns)) #initialize objective function with array of zeros
    obj[mainreaction[1]]=1 #add a 1 to the objective funciton at the index of the biomass reaction 

    result<-calculatemax(S,obj,lb,ub) #calculate solution for LP
    lp<-result$lp #LP object
    optval<-result$optval #objective value
    
    colvalues<-c() 
    results<-data.frame()
    objindex<-mainreaction[1] #index of biomassreaction 
    for(i in 1:S[[1]]@Dim[2]){ #fill an array with the rates of all reactions
      colvalues<-c(colvalues,getColPrimGLPK(lp,i))
    }
    colvalues<-c(colvalues[objindex],colvalues[-objindex]) #add biomass rate as first entry and bind it to the other values minus the biomass reaction
    reacnames<-c(rxns[objindex],rxns[-objindex]) #add biomass name as first entry
    results<-as.data.frame(cbind(reacnames,colvalues)) #bind reaction names and rates
    colnames(results)<-c("Reaction","Rate")
    write.table(results,output,sep="\t\t",col.names=FALSE,row.names=FALSE,quote = FALSE) #write results to the output file
    print(paste("output saved to",output)) 
  }
  else if(Biomass=="BLOCKED"){
    columns=length(rxns) 
    results<-c()
    for(i in 1:columns){ #calculate the optimal value for the maximization of each reaction.
      obj<-rep(0,columns)
      obj[i]=1
      optval=calculatemax(S,obj,lb,ub)$optval
      if(optval==0){ #If the optimal value equal 0 the reaction is blocked and the name is added to the result array
        results<-c(results,rxns[i])
      }
    }
    return(results) #return array containing the names of all blocked reactions
  }
}

