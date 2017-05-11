#include "fm.h"

//Update the gain value based on te and fs values in the cell
void updateGain() {
  unsigned int count = 0;
  for (count = 0; count < vertices; count++) {
    cell[count].gain = cell[count].fs - cell[count].te;
  }
}

//Update the area of the partition
void updatePartitionArea() {
  
  unsigned int count = 0;
  partition1_area = 0;
  partition2_area = 0;
  total_area = 0;

  for (count = 0; count < vertices; count++) {
    if(partition_max_area < cell[count].area) {
      partition_max_area = cell[count].area;
    }
    else {
      //do nothing
    }

    if(cell[count].partition == 1) {
      partition1_area = partition1_area + cell[count].area;
    }
    else {
      partition2_area = partition2_area + cell[count].area;
    }
  }
  total_area = partition1_area + partition2_area ;
  cout<<"Total Area : "<<total_area<<endl<<endl;
  cout<<"Max Area : "<<partition_max_area<<endl<<endl;
}

//Update the cells from given hypergraph
int initNodes() {

  FILE *fpread;
  unsigned int area = 0;
  unsigned int count = 0;
  unsigned int pointer = 0;
  char buffer[MAX_LENGTH_LINE];
  char partition2[MAX_LENGTH_LINE];
  char edge[MAX_LENGTH_LINE];
  int e1, e2, e3;

  fpread = fopen("hypergraph.txt","r"); 
  if( fpread == NULL ) {
    perror("Error while opening the file.\n");
    return UNSUCCESSFUL;
  }

  //  Ratio factor updation
  if(fgetc(fpread) == 'r') {
    fgets(buffer, MAX_LENGTH_LINE, fpread);  
    sscanf(buffer, "%lf", &ratio);
  }

  // Number of vertices updation
  if(fgetc(fpread) == 'n') {
    fgets(buffer, MAX_LENGTH_LINE, fpread);  
    sscanf(buffer, "%d", &vertices);
  }

  // All cells are updated with label, area value and partition - 1
  for (count = 0; count < vertices; count++) {
    if(fgetc(fpread) == (int)(97+count)) {
      fgets(buffer, MAX_LENGTH_LINE, fpread);  
      sscanf(buffer, "%d", &area);
      cell[count].label = (97+count);
      cell[count].area = area;
      cell[count].partition = 1;
      cell[count].gain = 0;
      cell[count].fs = 0;
      cell[count].te = 0;
      cell[count].temp_lock = 0;
    }    
  }

  // Bi-partitioning updation for partition 2
  if(fgetc(fpread) == 'B') {
    fgets(buffer, MAX_LENGTH_LINE, fpread);  
    sscanf(buffer, "%s", partition2);
    for (count = 0; count < vertices; count++) {
      if (cell[count].label == partition2[pointer]) {
        cell[count].partition = 2;
        pointer++;
      }
    }
  }

  //Retention Force(te) and Moving Force(fs) - updating
  while (fgetc(fpread) != EOF){
    fseek(fpread, -1, SEEK_CUR);

    //Edges connecting only two vertices
    if(fgetc(fpread) == 'x') {
      fgets(buffer, MAX_LENGTH_LINE, fpread);  
      sscanf(buffer, "%s", edge);
      e1 = ((int)edge[0]-97);
      e2 = ((int)edge[1]-97);
      if(cell[e1].partition == cell[e2].partition) {
        cell[e1].te++; //uncut edges
        cell[e2].te++;
      }
      else {
        cell[e1].fs++; //crossing edges
        cell[e2].fs++;
      }
    }
    

    else if ((fseek(fpread, -1, SEEK_CUR) || fgetc(fpread) == 'y')) {
      fgets(buffer, MAX_LENGTH_LINE, fpread);  
      sscanf(buffer, "%s", edge);
      e1 = ((int)edge[0]-97);
      e2 = ((int)edge[1]-97);
      e3 = ((int)edge[2]-97);
      if(cell[e1].partition == cell[e2].partition == cell[e3].partition) {
        cell[e1].te++; //uncut edges
        cell[e2].te++;
        cell[e3].te++;
      }
      else if(cell[e1].partition == cell[e2].partition) {
        cell[e3].fs++; //crossing edges
      }
      else if(cell[e1].partition == cell[e3].partition) {
        cell[e2].fs++; //crossing edges
      }
      else {
        cell[e1].fs++; //crossing edges
      }
    }

    else {
      //Do nothing
    }

  }
  
  //Update Gain
  updateGain();

  //Update partition area
  updatePartitionArea();

  fclose(fpread);

  return SUCCESSFUL;
}

//Update the nodes after every iteration
int updateNodes() {

  FILE *fpread;
  char buffer[MAX_LENGTH_LINE];
  char edge[MAX_LENGTH_LINE];
  int e1, e2, e3;
  unsigned int count = 0;

  fpread = fopen("hypergraph.txt","r"); 
  if( fpread == NULL ) {
    perror("Error while opening the file.\n");
    return UNSUCCESSFUL;
  }

  for (count = 0; count < vertices; count++) {
    cell[count].te = 0;
    cell[count].fs = 0;
  } 

  while (fgetc(fpread) != 'x') {
  }

  //Retention Force(te) and Moving Force(fs) - updating
  while (fgetc(fpread) != EOF){
    fseek(fpread, -1, SEEK_CUR);

    //Edges connecting only two vertices
    if(fgetc(fpread) == 'x') {
      fgets(buffer, MAX_LENGTH_LINE, fpread);
      sscanf(buffer, "%s", edge);
      e1 = ((int)edge[0]-97);
      e2 = ((int)edge[1]-97);
      if(cell[e1].partition == cell[e2].partition) {
        cell[e1].te++; //uncut edges
        cell[e2].te++;
      }
      else {
        cell[e1].fs++; //crossing edges
        cell[e2].fs++;
      }
    }
    

    else if ((fseek(fpread, -1, SEEK_CUR) || fgetc(fpread) == 'y')) {
      fgets(buffer, MAX_LENGTH_LINE, fpread);  
      sscanf(buffer, "%s", edge);
      e1 = ((int)edge[0]-97);
      e2 = ((int)edge[1]-97);
      e3 = ((int)edge[2]-97);
      if(cell[e1].partition == cell[e2].partition == cell[e3].partition) {
        cell[e1].te++; //uncut edges
        cell[e2].te++;
        cell[e3].te++;
      }
      else if(cell[e1].partition == cell[e2].partition) {
        cell[e3].fs++; //crossing edges
      }
      else if(cell[e1].partition == cell[e3].partition) {
        cell[e2].fs++; //crossing edges
      }
      else {
        cell[e1].fs++; //crossing edges
      }
    }

    else {
      //Do nothing
    }

  }
  
  //Update Gain
  updateGain();

  fclose(fpread);
  return SUCCESSFUL;
}

//Checking balance criteria 
int  b_criterion (int area_max_gain, int partition_number) 	{
	
	double partition1_area_t, partition2_area_t;

	if(partition_number == 1) {
    partition1_area_t = partition1_area - area_max_gain;
    if((((ratio * total_area) - partition_max_area) <=  partition1_area_t) && (((ratio * total_area) + partition_max_area) >=  partition1_area_t) ) {
        return SUCCESSFUL;
	   }
  }

	else {
		partition1_area_t = partition1_area + area_max_gain;
    if((((ratio * total_area) - partition_max_area) <=  partition1_area_t) && (((ratio * total_area) + partition_max_area) >=  partition1_area_t) ) {
        return SUCCESSFUL;
     }
	}

	return UNSUCCESSFUL;	
}

//Print the Bucket array
void printBucket(std::vector<std::list<nodes> > adjList) {

    int bucket_array_index;
    std::vector<std::list<nodes> >::iterator ba_ptr;
    std::list<nodes> li;
    std::list<nodes>::iterator iter;

    //Traversing the whole bucket
    
    bucket_array_index = PMAX;
    for (ba_ptr = adjList.begin(); ba_ptr != adjList.end(); ++ba_ptr)    {
        cout<<"Bucket array with gain "<<"|"<<bucket_array_index <<"|";
        li = *ba_ptr;
        for(iter = li.begin(); iter != li.end(); ++iter){
                cout<<" --->" <<" cell:"<<iter->partition<<" "<<iter->label;
        }
        cout<<endl;
        bucket_array_index--;
    }
}

//Retrieval of the best-gain (feasible) move 
char bestGainCal(std::vector<std::list<nodes> > *adjList)	{

	int lock_ret = -1;
  char fixed_cell = 0;
  std::vector<std::list<nodes> >::iterator ba_ptr;
  std::list<nodes> li;
  std::list<nodes>::iterator iter;
    
  for (ba_ptr = adjList->begin(); ((ba_ptr != (adjList->end()) && (lock_ret != 1))) ; ++ba_ptr)	{
  	li = *ba_ptr;
      for(iter = li.begin(); ((iter != li.end()) && (lock_ret != 1)); ++iter){
          //Checking balance criteria
      	lock_ret = b_criterion(iter->area, iter->partition);
      	if(lock_ret == 1){
          fixed_cell = iter->label;
          return fixed_cell;  
      	}
    		else{
        			//Do nothing
    		}	
      }
    }
  return UNSUCCESSFUL;
}

//Fixed Cell partition change
int fixCellUpdation(char fixed_cell){
  
  if(fixed_cell != 0){
      if(cell[fixed_cell-97].partition == 1){
          cell[fixed_cell-97].partition = 2;
          partition1_area = partition1_area - cell[fixed_cell-97].area;
          partition2_area = partition2_area + cell[fixed_cell-97].area;  
      }
      else{
          cell[fixed_cell-97].partition = 1;
          partition1_area = partition1_area + cell[fixed_cell-97].area;
          partition2_area = partition2_area - cell[fixed_cell-97].area;
      }
    //Locking the current fixed cell
    cell[fixed_cell-97].temp_lock = 1;
    return SUCCESSFUL;
  }
  else{
    return UNSUCCESSFUL;
  }
}

//RatioFactor(partition1) calculation for better movement at end of the pass
double ratioFactorCalculation(char fixed_cell) {

  double rf_t = 0;
  if(cell[(fixed_cell-97)].partition == 1)
    partition1_area = partition1_area - cell[fixed_cell-97].area ;
  else
    partition1_area = partition1_area + cell[fixed_cell-97].area ; 

  rf_t = (double)partition1_area / (double)total_area;
  
  return rf_t;
}