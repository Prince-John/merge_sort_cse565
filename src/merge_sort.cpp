#include <hls_stream.h>

static void comparator(int block_size, hls::stream<int> &fifo1, hls::stream<int> &fifo2, hls::stream<int> &fifo_OUT)
{

			int temp_int_1;
			int temp_int_2;
			int temp_1_flag =0;
			int temp_2_flag =0;

			int i = block_size;
			int j = block_size;



			while(i!=0 && j!=0 ){

					if(temp_1_flag == 0){
						temp_int_1 = fifo1.read();
						temp_1_flag = -1;
					}
					if(temp_2_flag == 0){		
						temp_int_2 = fifo2.read();
						temp_2_flag = -1;
					}
					
					if(temp_int_1>temp_int_2){
						fifo_OUT.write(temp_int_2);
						//fifo_OUT.write(temp_int_1);
						temp_2_flag = 0;
						j--;
					}
					else{
						fifo_OUT.write(temp_int_1);
						//fifo_OUT.write(temp_int_2);
						temp_1_flag = 0;
						i--;
					}
										
					

				}

			while(i!=0 ){

					if(temp_1_flag == 0){
							temp_int_1 = fifo1.read();
						}
					fifo_OUT.write(temp_int_1);
					temp_1_flag = 0;
					i--;
				}

			while(j!=0 ){

					if(temp_2_flag == 0){
							temp_int_2 = fifo2.read();
						}
					fifo_OUT.write(temp_int_2);
					temp_2_flag = 0;
					j--;
				}
}

static void comparator_stage_1(int temp_int_1, int temp_int_2, hls::stream<int> &fifo_OUT)
{
		if(temp_int_1>temp_int_2){
			fifo_OUT.write(temp_int_2);
			fifo_OUT.write(temp_int_1);
		}
		else{
			fifo_OUT.write(temp_int_1);
			fifo_OUT.write(temp_int_2);
		}


	}



static void stream_to_output(
	int size, int start,
	hls::stream<int> &fifo_in0,
	int *buffer
	){
		int i = start;
		while(!fifo_in0.empty()){
			buffer[i] = fifo_in0.read();
			i++;
		}

	}



static void allocate_to_next_fifo(
	int size,
	int block_size,
	hls::stream<int> &fifo_in0, 
	hls::stream<int> &fifo_in1,
	hls::stream<int> &fifo_out0,
	hls::stream<int> &fifo_out1)
{
	for(int i = 0; i< size/(block_size*2); i++){

	       
		if(i%2 == 0){
					comparator(block_size, fifo_in0, fifo_in1, fifo_out0);
				}
				else{
					comparator(block_size, fifo_in0, fifo_in1, fifo_out1);
    
        }
    }
	

}

static void allocate_to_next_fifo_1(
	int size,
	unsigned int *data0,
	hls::stream<int> &fifo_out0,
	hls::stream<int> &fifo_out1)
{	
	int j = 0;

	for(int i = 0, j = 0; i< size/2; i++){
		
		#pragma HLS unroll


		if(i%2 == 0){
			comparator_stage_1(data0[j],data0[j+1], fifo_out0);
		}
		else{
			comparator_stage_1(data0[j],data0[j+1], fifo_out1);
		}
		
		j+=2;

	}

}

static void read_to_fully_partitioned_array(
	int size,
	const unsigned int *data0,
	unsigned int *data1)
{	
		
	/*    reading into on chip memory       */

	//Starting the loading into a fully partitioned buffer. 

	for(int i = 0; i < size; i++){
	//	#pragma HLS unroll	
		data1[i] = data0[i];
	}	


}





extern "C" {
	void merge_sort(
	        const unsigned int *in1, // Read-Only Vector 1
	        int *out       // Output Result
		)
	
	{
			const int size = 512;// Size in integer
			unsigned int data0[size];

#pragma HLS INTERFACE m_axi port=in1 bundle=aximm1
#pragma HLS INTERFACE m_axi port=out bundle=aximm2
#pragma HLS array_partition variable=data0 type = complete




			
			static hls::stream<int> stream_1_stage_1("fifo_1_stage_1");
			#pragma HLS stream variable= stream_1_stage_1 depth=256
			static hls::stream<int> stream_2_stage_1("fifo_2_stage_1");
			#pragma HLS stream variable= stream_2_stage_1   depth=256
			static hls::stream<int> stream_1_stage_2("fifo_1_stage_2");
			#pragma HLS stream variable= stream_1_stage_2  depth=256
			static hls::stream<int> stream_2_stage_2("fifo_2_stage_2");
			#pragma HLS stream variable= stream_2_stage_2  depth=256
			static hls::stream<int> stream_1_stage_3("fifo_1_stage_3");
			#pragma HLS stream variable= stream_1_stage_3  depth=256
			static hls::stream<int> stream_2_stage_3("fifo_2_stage_3");
			#pragma HLS stream variable= stream_2_stage_3  depth=256
			static hls::stream<int> stream_1_stage_4("fifo_1_stage_4");
			#pragma HLS stream variable= stream_1_stage_4  depth=256
			static hls::stream<int> stream_2_stage_4("fifo_2_stage_4");
			#pragma HLS stream variable= stream_2_stage_4  depth=256
			static hls::stream<int> stream_1_stage_5("fifo_1_stage_5");
			#pragma HLS stream variable= stream_1_stage_5  depth=256
			static hls::stream<int> stream_2_stage_5("fifo_2_stage_5");
			#pragma HLS stream variable= stream_2_stage_5  depth=256
			static hls::stream<int> stream_1_stage_6("fifo_1_stage_6");
			#pragma HLS stream variable= stream_1_stage_6  depth=256
			static hls::stream<int> stream_2_stage_6("fifo_2_stage_6");
			#pragma HLS stream variable= stream_2_stage_6  depth=256
			static hls::stream<int> stream_1_stage_7("fifo_1_stage_7");
			#pragma HLS stream variable= stream_1_stage_7  depth=256
			static hls::stream<int> stream_2_stage_7("fifo_2_stage_7");
			#pragma HLS stream variable= stream_2_stage_7  depth=256		
			static hls::stream<int> stream_1_stage_8("fifo_1_stage_8");
			#pragma HLS stream variable= stream_1_stage_8  depth=256
			static hls::stream<int> stream_2_stage_8("fifo_2_stage_8");
			#pragma HLS stream variable= stream_2_stage_8  depth=256
			// static hls::stream<int> stream_1_stage_9("fifo_1_stage_9");
			// #pragma HLS stream variable= stream_1_stage_9  depth=256
			// static hls::stream<int> stream_2_stage_9("fifo_2_stage_9");
			// #pragma HLS stream variable= stream_2_stage_9  depth=256
			static hls::stream<int> stream_stage_10("fifo_stage_10");
			#pragma HLS stream variable= stream_stage_10  depth=512



		read_to_fully_partitioned_array(size,in1,data0);


/*******************************************************************************************/
/**************************************STAGE1***********************************************/
/*******************************************************************************************/
			
		{			
			#pragma HLS dataflow 
			
			allocate_to_next_fifo_1(size, data0, stream_1_stage_1, stream_2_stage_1);


/*******************************************************************************************/
/**************************************STAGE2***********************************************/
/*******************************************************************************************/

		
			

			int block_size = 2;

			allocate_to_next_fifo(size, block_size, stream_1_stage_1, stream_2_stage_1, stream_1_stage_2, stream_2_stage_2);

		
/*******************************************************************************************/
/**************************************STAGE3***********************************************/
/*******************************************************************************************/

			
		
			block_size = 4;

			allocate_to_next_fifo(size, block_size, stream_1_stage_2, stream_2_stage_2, stream_1_stage_3, stream_2_stage_3);


// /*******************************************************************************************/
// /**************************************STAGE4***********************************************/
// /*******************************************************************************************/
// 			


			block_size = 8;

			allocate_to_next_fifo(size, block_size, stream_1_stage_3, stream_2_stage_3, stream_1_stage_4, stream_2_stage_4);


// /*******************************************************************************************/
// /**************************************STAGE5***********************************************/
// /*******************************************************************************************/

			
		
			block_size = 16;
			allocate_to_next_fifo(size, block_size, stream_1_stage_4, stream_2_stage_4, stream_1_stage_5, stream_2_stage_5);




// /*******************************************************************************************/
// /**************************************STAGE6***********************************************/
// /*******************************************************************************************/


			block_size = 32;
			allocate_to_next_fifo(size, block_size, stream_1_stage_5, stream_2_stage_5, stream_1_stage_6, stream_2_stage_6);



// /*******************************************************************************************/
// /**************************************STAGE7***********************************************/
// /*******************************************************************************************/

 			
		

			block_size = 64;
			allocate_to_next_fifo(size, block_size, stream_1_stage_6, stream_2_stage_6, stream_1_stage_7, stream_2_stage_7);


// /*******************************************************************************************/
// /**************************************STAGE8***********************************************/
// /*******************************************************************************************/

			block_size = 128;
			allocate_to_next_fifo(size, block_size, stream_1_stage_7, stream_2_stage_7, stream_1_stage_8, stream_2_stage_8);


// /*******************************************************************************************/
// /**************************************STAGE9***********************************************/
// /*******************************************************************************************/

			
			
			// block_size = 256;
			
			// allocate_to_next_fifo(size, block_size, stream_1_stage_8, stream_2_stage_8, stream_1_stage_9, stream_2_stage_9);


			


/*******************************************************************************************/
/**************************************STAGE10***********************************************/
/*******************************************************************************************/
			
			
			comparator(256, stream_1_stage_8, stream_2_stage_8, stream_stage_10);
			

/*******************************************************************************************/
/***********************************Write out to BUFFER*************************************/
/*******************************************************************************************/		
			
			
			stream_to_output(512,0,stream_stage_10,out);

		}	


	}


}
