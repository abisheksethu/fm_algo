/******************
Filename: fmalgorithm.c
Author: Sethupandi Abishek
******************/
#include "fm.h"

int main() {

    unsigned int count, location;
    unsigned int iteration;
    unsigned int cumulative_gain = 0;
    char fixed_cell = 0;
    std::vector<std::list<nodes> > adjList((PMAX*2)+1);

    cout<<endl<<endl<<"*****************************FM ALGORITHM*********************************"<<endl;
    cout<<"****************************************************************************"<<endl<<endl;
    cout<<"Graph details: "<<endl<<endl;

    //Initial updation of Nodes
    initNodes();

    //Pushback to bucket array with respect to gain values
      for (count = 0; count < vertices; count++) {
            cout<<"Cell-->"<<cell[count].label<<" Gain-->"<<cell[count].gain;
            cout<<" Area-->"<<cell[count].area<<" Partition-->"<<cell[count].partition;
            cout<<" te-->"<<cell[count].te<<" fs-->"<<cell[count].fs; 
            cout<<endl;
          adjList[PMAX - (cell[count].gain)].push_back(cell[count]);  //node: cell[count].label with gain
      }

    //FM iteration routine --> number of vertices as stepsize
    for (iteration = 0; iteration < vertices; iteration++) {

        //Print the Bucket array
        cout<<endl<<"Iteration : "<<iteration+1<<endl<<endl;
        #if DEBUG   
        printBucket(adjList);    
        #endif

        //Best gain identification 
        fixed_cell = bestGainCal(&adjList);
        cout<<endl<<"Selected fixed cell: " <<" cell "<<fixed_cell<<endl<<endl;


        //Fix the cell by locking
        if(fixCellUpdation(fixed_cell) == 0)
            return 0;

        //Popback all the nodes 
        for (count = 0; count < vertices; count++) {
            if(!adjList[PMAX - (cell[count].gain)].empty())
                adjList[PMAX - (cell[count].gain)].pop_back();
            else{
                //Do nothing
            }
        }

        //Update the Gain value for Cumulative Gain calculation
        gain_list[iteration].gain = cell[fixed_cell-97].gain;
        gain_list[iteration].fixed_cell = fixed_cell;


        //TE, FS and Gain updation 
        updateNodes();
        

        //Pushback with respect to new gain values except locked cell
        for (count = 0; count < vertices; count++) {
            if((cell[count].temp_lock) != 1){
                adjList[PMAX - (cell[count].gain)].push_back(cell[count]);
            #if DEBUG
                cout<<"Cell-->"<<cell[count].label<<" Gain-->"<<cell[count].gain;
                cout<<" Area-->"<<cell[count].area<<" Partition-->"<<cell[count].partition;
                cout<<" te-->"<<cell[count].te<<" fs-->"<<cell[count].fs; 
                cout<<endl;
            #endif
            }
            else{
                //Do nothing
            }
        }

    }

    cout<<endl<<endl<<"*************************REPORT FOR PASS 1*******************************"<<endl;
    cout<<"*************************************************************************"<<endl<<endl;
   
    //Reverting all nodes as per initial values
    initNodes();

    //Cumulative Gain Suggestion Report - Pass 1
    for(count = 0; count < vertices; count++) {
        cumulative_gain = cumulative_gain + gain_list[count].gain;
        gain_list[count].cumulative_gain = cumulative_gain;
        gain_list[count].ratio_factor = ratioFactorCalculation(gain_list[count].fixed_cell);
        cout<<"Cumulative Gain for iteration"<<count+1<<" = "<<gain_list[count].cumulative_gain<<" Partition1 RF "<<gain_list[count].ratio_factor<<endl<<endl;
    }

    //Selection of cumulative gain
    cumulative_gain = 0;
    for(count = 0; count < vertices; count++){
        if(cumulative_gain <= (gain_list[count].cumulative_gain)){
            cumulative_gain = gain_list[count].cumulative_gain;
            location = count;
        }
        else {
            //Do nothing
        }
    } 
    cout<<"Selected cumulative gain is "<<cumulative_gain<<endl<<endl;
    cout<<"nodes selected for movement(partition no) ";
    for(count = 0; count <= (location); count++) {
        cout<<gain_list[count].fixed_cell;
        if(cell[(gain_list[count].fixed_cell)-97].partition == 1){
            cout<<"(2)  ";
        }
        else {
            cout<<"(1)  ";
        }
    }
    cout<<endl<<endl;

    //partition information;


    cout<<endl<<endl<<"******************************END****************************************"<<endl;
    cout<<"*************************************************************************"<<endl<<endl;

    return 0;
}