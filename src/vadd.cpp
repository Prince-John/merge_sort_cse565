extern "C" {
	void merge_sort(
	        const unsigned int *in1, // Read-Only Vector 1
	        int *out,       // Output Result
		)
	{
	const int size = 1024;// Size in integer

#pragma HLS INTERFACE m_axi port=in1 bundle=aximm1
#pragma HLS INTERFACE m_axi port=out bundle=aximm1

	    for(int i = 0; i < size; ++i)
	    {
	        out[i] = in1[i] + in2[i];
	    }

	}
}

