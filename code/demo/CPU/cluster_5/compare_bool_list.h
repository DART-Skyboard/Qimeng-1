int compare_bool_list(bool*data_a,bool*data_b,int list_length){

	int	compare_result = 0;

	int 	i;
	for (i=0;i<list_length;i++){
		if((data_a[i]) && (!data_b[i])){
			compare_result = 1;
			break;
		}else if((!data_a[i]) && data_b[i]){
			compare_result = 2;
			break;
		}
	}
	//for(i=0;i<list_length;i++){
	//	cout<<data_a[i];
	//}
	//cout<<"  ";
	//for(i=0;i<list_length;i++){
	//	cout<<data_b[i][which_line];
	//}
	//cout<<"  "<<compare_result<<endl;
	return	compare_result;

}
