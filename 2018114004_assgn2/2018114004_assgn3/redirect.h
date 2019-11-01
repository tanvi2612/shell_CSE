void redirect(int tot, char *arr[]){
    i_chk = 0;
    o_chk = 0;
    
     
    for(int i =0; i<tot; i++){
      //  printf("halo\n");
        if(!strcmp(arr[i],"<")){
            // input file
            if(arr[i+1] == NULL){
                perror("wrong syntax");
            }
            arr[i] = NULL;
            i_chk = 1;
            strcpy(i_file, arr[i+1]);
            var_1 = open(i_file, O_RDONLY);
        }
        
        else if(!strcmp(arr[i],">")){
            // output file
            if(arr[i+1] == NULL){
                perror("wrong syntax");
            }
            arr[i] = NULL;
            o_chk = 1;
            strcpy(o_file, arr[i+1]);
            var_2= open(o_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            
        }
        else if(!strcmp(arr[i],">>")){
            // append into file
            if(arr[i+1] == NULL){
                perror("wrong syntax");
            }
            arr[i] = NULL;
            o_chk = 1;
            strcpy(o_file, arr[i+1]);
            var_2= open(o_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
        }
        
    }
    //printf("halo2\n");
    // if(i_chk == 1){
    //    var_1 = open(i_file, O_RDONLY);
    //    if(var_1 == -1){
    //        perror("Error in opening file");
    //        return;
    //    }
    //    dup2(var_1,0);
       
    //    close(var_1);
    // }
    // if(o_chk == 1){
    //    var_2 = open(o_file, O_WRONLY | O_TRUNC | O_CREAT);
    //    if(var_3 == -1){
    //        perror("Error in opening file");
    //        return;
    //    }
    //    dup2(var_2,1);
       
    //    close(var_2);
    // }
    // if(a_chk == 1){
        
    //    var_3 = open(a_file, O_WRONLY |O_APPEND);
    //    if(var_3 == -1){
    //        perror("Error in opening file");
    //        return;

    //    }
    //    dup2(var_3,1);
    //    //printf("halo3\n");
      
    //    close(var_3);
    // }
    
    return;

}