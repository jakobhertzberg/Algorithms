
calculatemax<-function(S,obj,lb,ub){
  #Initialize lp and set row bounds
  lp <- initProbGLPK()
  setObjDirGLPK(lp, GLP_MAX)
  rows<-S[[1]]@Dim[1]
  columns<-S[[1]]@Dim[2]
  addRowsGLPK(lp,nrows=rows)
  addColsGLPK(lp,ncols=columns)
  lbrows <- rep(0,rows)
  ubrows <-rep(0,rows)
  type <- rep(GLP_FX,rows)
  setRowsBndsGLPK(lp, 1:rows, lb=lbrows, ub=ubrows, type)
  
  #set column bounds and objective function
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
  
  #load constraint matrix
  summary<-summary(S[[1]])
  ia <- summary$i
  ja <- summary$j
  ar <- summary$x
  loadMatrixGLPK(lp,length(ia), ia, ja, ar)
  
  
  solveSimplexGLPK(lp)
  optval<-getObjValGLPK(lp)
  return(list(lp=lp,optval=optval))
}

analyzefluxbal<-function(matfile,Biomass,output){
  library(R.matlab)
  library(glpkAPI)
  network<-readMat(matfile)$network
  S<-unlist(network[1])
  rev<-unlist(network[2])
  rxns<-unlist(network[3])
  lb<-unlist(network[4])
  ub<-unlist(network[5])
  description<-unlist(network[6])

  if(Biomass=="BIOMASS"){
    mainreaction<-c()
    for(i in 1:S[[1]]@Dim[2]){ #get biomass reaction and create obj function
      if(grepl("biomass",rxns[i],ignore.case = TRUE)){
        mainreaction<-c(mainreaction,i)
      }
    }

    obj<-rep(0,length(rxns))
    obj[mainreaction[1]]=1

    result<-calculatemax(S,obj,lb,ub) #calculate solution
    lp<-result$lp
    optval<-result$optval
    
    colvalues<-c()
    results<-data.frame()
    objindex<-mainreaction[1]
    getObjValGLPK(lp)
    for(i in 1:S[[1]]@Dim[2]){
      colvalues<-c(colvalues,getColPrimGLPK(lp,i))
    }
    colvalues<-c(colvalues[objindex],colvalues[-objindex])
    reacnames<-c(rxns[objindex],rxns[-objindex])
    results<-as.data.frame(cbind(reacnames,colvalues))
    colnames(results)<-c("Reaction","Rate")
    write.table(results,output,sep="\t\t",col.names=FALSE,row.names=FALSE,quote = FALSE)
    print(paste("output saved to",output))
  }
  else if(Biomass=="BLOCKED"){
    columns=length(rxns)
    results<-c()
    for(i in 1:columns){
      obj<-rep(0,columns)
      obj[i]=1
      optval=calculatemax(S,obj,lb,ub)$optval
      if(optval==0){
        results<-c(results,rxns[i])
      }
    }
    return(results)
  }
}

