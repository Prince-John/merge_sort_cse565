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

static void stream_to_output_test(
	int size, int start,
	hls::stream<int> &fifo_in0, hls::stream<int> &fifo_in1,
	int *buffer
	){

		int i = 0;

		for(i = 0; i<2;i++){

			buffer[i] = fifo_in0.read();
		}

		for(i = 3; i<4;i++){

			buffer[i] = fifo_in1.read();
			
		}

	}

// static void input_to_stream(
// 	int size,
// 	int *in_buffer,
// 	int *buffer
// 	){
// 		for(int i =0; i<size; i++){
// 			buffer[i] = fifo_in0.read();
// 		}

// 	}


static void allocate_to_next_fifo(
	int size,
	int block_size,
	hls::stream<int> &fifo_in0, 
	hls::stream<int> &fifo_in1,
	hls::stream<int> &fifo_out0,
	hls::stream<int> &fifo_out1)
{
	for(int i = 0; i< size/(block_size*4); i++){

		comparator(block_size, fifo_in0, fifo_in1, fifo_out0);
		
	}
	for(int i = size/(block_size*4); i< size/(block_size*2); i++){

		comparator(block_size, fifo_in0, fifo_in1, fifo_out1);
		
	}

}

static void allocate_to_next_fifo_1(
	int size,
	const unsigned int *data0,
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



extern "C" {
	void merge_sort(
	        const unsigned int *in1, // Read-Only Vector 1
	        int *out       // Output Result
		)
	{
			const int size = 4;// Size in integer
			int data0[size];

#pragma HLS INTERFACE m_axi port=in1 bundle=aximm1
#pragma HLS INTERFACE m_axi port=out bundle=aximm2

//#pragma HLS array_partition variable=in1 type = complete  
// #pragma HLS array_partition variable=data0 type = complete



			
// 			/*    reading into on chip memory       */

// 			//Starting the loading into a fully partitioned buffer. 
		
// 			for(int i = 0; i < size; i++){
// 			//	#pragma HLS unroll	
// 				data0[i] = in1[i];
// 			}	
			
			static hls::stream<int> stream_1_stage_1("fifo_1_stage_1");
			#pragma HLS stream variable= stream_1_stage_1 depth=2
			static hls::stream<int> stream_2_stage_1("fifo_2_stage_1");
			#pragma HLS stream variable= stream_2_stage_1   depth=2
			static hls::stream<int> stream_1_stage_2("fifo_1_stage_2");
			#pragma HLS stream variable= stream_1_stage_2  depth=4
			static hls::stream<int> stream_2_stage_2("fifo_2_stage_2");
			#pragma HLS stream variable= stream_2_stage_2  depth=4



/*******************************************************************************************/
/**************************************STAGE1***********************************************/
/*******************************************************************************************/
			
		{			
			#pragma HLS dataflow 
			
			allocate_to_next_fifo_1(size, in1, stream_1_stage_1, stream_2_stage_1);


/*******************************************************************************************/
/**************************************STAGE2***********************************************/
/*******************************************************************************************/


			comparator(4, stream_1_stage_2, stream_2_stage_2, stream_stage_10);
			
			

			// int block_size = 2;

			// allocate_to_next_fifo(size, block_size, stream_1_stage_1, stream_2_stage_1, stream_1_stage_2, stream_2_stage_2);

			
/*******************************************************************************************/
/**************************************STAGE3***********************************************/
/*******************************************************************************************/

// 			
// 			static hls::stream<int> stream_1_stage_3("fifo_1_stage_3");
// 			#pragma HLS stream variable= stream_1_stage_3  depth=8
// 			static hls::stream<int> stream_2_stage_3("fifo_2_stage_3");
// 			#pragma HLS stream variable= stream_2_stage_3  depth=8

// 			block_size = 4;

// 			allocate_to_next_fifo(size, block_size, stream_1_stage_2, stream_2_stage_2, stream_1_stage_3, stream_2_stage_3);


// /*******************************************************************************************/
// /**************************************STAGE4***********************************************/
// /*******************************************************************************************/
// 			

// 			static hls::stream<int> stream_1_stage_4("fifo_1_stage_4");
// 			#pragma HLS stream variable= stream_1_stage_4  depth=16
// 			static hls::stream<int> stream_2_stage_4("fifo_2_stage_4");
// 			#pragma HLS stream variable= stream_2_stage_4  depth=16

// 			block_size = 8;

// 			allocate_to_next_fifo(size, block_size, stream_1_stage_3, stream_2_stage_3, stream_1_stage_4, stream_2_stage_4);


// /*******************************************************************************************/
// /**************************************STAGE5***********************************************/
// /*******************************************************************************************/

// 			
// 			static hls::stream<int> stream_1_stage_5("fifo_1_stage_5");
// 			#pragma HLS stream variable= stream_1_stage_5  depth=32
// 			static hls::stream<int> stream_2_stage_5("fifo_2_stage_5");
// 			#pragma HLS stream variable= stream_2_stage_5  depth=32

// 			block_size = 16;
// 			for(int i = 0; i< size/32; i++){

// 				if(i%2 == 0){
// 					comparator(block_size, stream_1_stage_4, stream_2_stage_4, stream_1_stage_5);
// 				}
// 				else{
// 					comparator(block_size, stream_1_stage_4, stream_2_stage_4, stream_2_stage_5);
// 				}
				

// 			}	


// /*******************************************************************************************/
// /**************************************STAGE6***********************************************/
// /*******************************************************************************************/

// 			
// 			static hls::stream<int> stream_1_stage_6("fifo_1_stage_6");
// 			#pragma HLS stream variable= stream_1_stage_6  depth=64
// 			static hls::stream<int> stream_2_stage_6("fifo_2_stage_6");
// 			#pragma HLS stream variable= stream_2_stage_6  depth=64

// 			block_size = 32;
// 			for(int i = 0; i< size/64; i++){
			
// 			#pragma HLS
// 				if(i%2 == 0){
// 					comparator(block_size, stream_1_stage_5, stream_2_stage_5, stream_1_stage_6);
// 				}
// 				else{
// 					comparator(block_size, stream_1_stage_5, stream_2_stage_5, stream_2_stage_6);
// 				}
				

// 			}


// /*******************************************************************************************/
// /**************************************STAGE7***********************************************/
// /*******************************************************************************************/

// 			
// 			static hls::stream<int> stream_1_stage_7("fifo_1_stage_7");
// 			#pragma HLS stream variable= stream_1_stage_7  depth=128
// 			static hls::stream<int> stream_2_stage_7("fifo_2_stage_7");
// 			#pragma HLS stream variable= stream_2_stage_7  depth=128

// 			block_size = 64;
// 			for(int i = 0; i< size/128; i++){

// 				if(i%2 == 0){
// 					comparator(block_size, stream_1_stage_6, stream_2_stage_6, stream_1_stage_7);
// 				}
// 				else{
// 					comparator(block_size, stream_1_stage_6, stream_2_stage_6, stream_2_stage_7);
// 				}
				

// 			}

// /*******************************************************************************************/
// /**************************************STAGE8***********************************************/
// /*******************************************************************************************/

// 			
// 			static hls::stream<int> stream_1_stage_8("fifo_1_stage_8");
// 			#pragma HLS stream variable= stream_1_stage_8  depth=256
// 			static hls::stream<int> stream_2_stage_8("fifo_2_stage_8");
// 			#pragma HLS stream variable= stream_2_stage_8  depth=256

// 			block_size = 128;
// 			for(int i = 0; i< size/256; i++){

// 				if(i%2 == 0){
// 					comparator(block_size, stream_1_stage_7, stream_2_stage_7, stream_1_stage_8);
// 				}
// 				else{
// 					comparator(block_size, stream_1_stage_7, stream_2_stage_7, stream_2_stage_8);
// 				}
				

// 			}

// /*******************************************************************************************/
// /**************************************STAGE9***********************************************/
// /*******************************************************************************************/

// 			
// 			static hls::stream<int> stream_1_stage_9("fifo_1_stage_9");
// 			#pragma HLS stream variable= stream_1_stage_9  depth=512
// 			static hls::stream<int> stream_2_stage_9("fifo_2_stage_9");
// 			#pragma HLS stream variable= stream_2_stage_9  depth=512

// 			block_size = 256;
			
// 			if ((size/512) <=0){


// 			}

			
// 			for(int i = 0; i< size/512; i++){

// 				if(i%2 == 0){
// 					comparator(block_size, stream_1_stage_8, stream_2_stage_8, stream_1_stage_9);
// 				}
// 				else{
// 					comparator(block_size, stream_1_stage_8, stream_2_stage_8, stream_2_stage_9);
// 				}

// 			}

			


/*******************************************************************************************/
/**************************************STAGE10***********************************************/
/*******************************************************************************************/
			
			// static hls::stream<int> stream_stage_10("fifo_stage_10");
			// #pragma HLS stream variable= stream_stage_10  depth=8

			//comparator(4, stream_1_stage_2, stream_2_stage_2, stream_stage_10);
			// stream_to_output(4, 0, stream_1_stage_2, out);
			
			// stream_stage_10.write(-1);
			// stream_stage_10.write(-1);
			// stream_stage_10.write(-1);
			// stream_stage_10.write(-1);
			// stream_stage_10.write(-1);
			// stream_stage_10.write(-1);
			// stream_stage_10.write(-1);
			// stream_stage_10.write(-1);
			
			
			
			stream_to_output_test(4,0, stream_1_stage_1, stream_2_stage_1, out);

			// stream_to_output(8,0,stream_stage_10,out);

		}	

/*******************************************************************************************/
/***********************************Write out to BUFFER*************************************/
/*******************************************************************************************/
			
	
			// int i = 0;
			// while(!stream_stage_10.empty()){
			// 	out[i] = stream_stage_10.read();
			// 	i++;

			// }
			


	}




}
