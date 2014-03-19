
//int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf,
//                       int buf_size) {
//    static AVPacket pkt;
//    static uint8_t *audio_pkt_data = NULL;
//    static int audio_pkt_size = 0;
//    int len1, data_size;
//    for(;;) {
//        while(audio_pkt_size > 0) {
//            data_size = buf_size;
//            len1 = avcodec_decode_audio2(aCodecCtx, (int16_t *)audio_buf, &data_size,
//                                            audio_pkt_data, audio_pkt_size);
//            if(len1 < 0) {
//                audio_pkt_size = 0;
//                break;
//            }
//            audio_pkt_data += len1;
//            audio_pkt_size -= len1;
//            if(data_size <= 0) {
//                continue;
//            }
//            return data_size;
//        }
//        if(pkt.data)
//            av_free_packet(&pkt);
//        if(quit) {
//            return -1;
//        }
//        if(packet_queue_get(&audioq, &pkt, 1) < 0) {
//            return -1;
//        }
//        audio_pkt_data = pkt.data;
//        audio_pkt_size = pkt.size;
//    }
//}
