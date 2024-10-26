#include <Windows.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <clap/clap.h>
#include <clap/process.h>

#define BUFFER_SIZE 8

static clap_process_status my_plug_process(const struct clap_plugin* plugin,
	const clap_process_t* process) {

	const uint32_t nframes = process->frames_count;

	for (uint32_t i = 0; i < nframes;) {
		// fetch input samples
		const float in_l = process->audio_inputs[0].data32[0][i];
		const float in_r = process->audio_inputs[0].data32[1][i];

		/* TODO: process samples, here we simply swap left and right channels */
		const float out_l = in_r;
		const float out_r = in_l;

		// store output samples
		process->audio_outputs[0].data32[0][i] = out_l;
		process->audio_outputs[0].data32[1][i] = out_r;
	}

	return CLAP_PROCESS_CONTINUE;
}

void ClapAudioBuffer_init(BYTE* pCaptureData)
{
	std::cout << "ClapAudioBuffer.size:" << BUFFER_SIZE << std::endl;

	clap_process process_data = {};

	clap_audio_buffer input_buffer[1] = { 0 };

	//input_buffer[0].data32 = reinterpret_cast<float**>(&pCaptureData);
	input_buffer[0].data32 = (float**) pCaptureData;
	input_buffer[0].channel_count = 2;
	//solution:input_buffer[0].data32 = reinterpret_cast<float**>(&pCaptureData);

	process_data.audio_inputs = &input_buffer[0];
	//process_data.audio_outputs = &output_buffer[0];
	process_data.audio_inputs_count = 1;
	process_data.audio_outputs_count = 1;

	process_data.frames_count = BUFFER_SIZE;

#if 0
	for (UINT i = 0; i < BUFFER_SIZE; i++)
	{
		//std::cout << "ClapAudioBuffer:" << pCaptureData[i] << std::endl;

		printf("%u: ClapAudioBuffer: %d\n", i, pCaptureData[i]);
	}
#endif

	(void) my_plug_process(nullptr, &process_data);
}




int main()
{
    std::cout << "Hello ClapAudioBuffer!\n";

	BYTE pCaptureData[BUFFER_SIZE];

	for (UINT i = 0; i < BUFFER_SIZE; i++)
	{
		pCaptureData[i] = i;
	}
	BYTE* pCaptureData2 = pCaptureData;

	//BYTE* act2dim[2];
	//act2dim[0] = (BYTE*)pCaptureData2;
	//act2dim[1] = (BYTE*)pCaptureData2 + BUFFER_SIZE / 2;

	//for (UINT i = 0; i < BUFFER_SIZE / 2; i++) {
	//	printf("%u-0: %d\n", i, act2dim[0][i]);
	//	printf("%u-1: %d\n", i, act2dim[1][i]);
	//}

	// Now reordering the buffer
	BYTE** pReorderedData = new BYTE * [2];
	pReorderedData[0] = new BYTE[BUFFER_SIZE / 2];
	pReorderedData[1] = new BYTE[BUFFER_SIZE / 2];

	for (UINT i = 0; i < BUFFER_SIZE; i++)
	{
		printf("%u: %d\n", i, pCaptureData2[i]);
	}

	for (UINT i = 0; i < BUFFER_SIZE / 2; i++) {
		pReorderedData[0][i] = pCaptureData2[i * 2];
		pReorderedData[1][i] = pCaptureData2[i * 2 + 1];

		printf("%u: %d,%d:%d,%d => %d,%d\n", i, i * 2, i * 2 + 1,
			pCaptureData2[i * 2],
			pCaptureData2[i * 2 + 1],
			pReorderedData[0][i],
			pReorderedData[1][i]
		);
	}

	for (UINT i = 0; i < BUFFER_SIZE / 2; i++) {
		printf("%u-0: %d\n", i, pReorderedData[0][i]);
		printf("%u-1: %d\n", i, pReorderedData[1][i]);
	}

	//BYTE** test2dim;
	//test2dim = reinterpret_cast<BYTE**>(&pCaptureData2);
	//for (UINT i = 0; i < BUFFER_SIZE / 2; i++) {
	//	printf("%u-0: %d\n", i, test2dim[0][i]);
	//	printf("%u-1: %d\n", i, test2dim[1][i]);
	//}

	//ClapAudioBuffer_init(pCaptureData);
	ClapAudioBuffer_init((BYTE*) pReorderedData);

	return 0;
}
