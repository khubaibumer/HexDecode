# Hex Decoder

[![CodeFactor](https://www.codefactor.io/repository/github/khubaibumer/hexdecode/badge)](https://www.codefactor.io/repository/github/khubaibumer/hexdecode)
[![Softacheck](https://softacheck.com/app/repository/khubaibumer/HexDecode/badge)](https://softacheck.com/app/repository/khubaibumer/HexDecode/issues)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/d0468f238b664c5b8445183913c94c43)](https://www.codacy.com/gh/khubaibumer/HexDecode/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=khubaibumer/HexDecode&amp;utm_campaign=Badge_Grade)

`decoder_t* get_thread_local_decoder()` <br>
Returns a per-thread Decoder. User can create as many decoders as threads and they will decode in a thread-safe way

`decoder_t* get_decoder(void)`<br>
Returns instance of Decoder <br>
User can create as many decoders as they like

## User Application APIs
Following APIs are provided by decoder
 * `get_decoder()/get_thread_local_decoder()` <br>
To Get instance of Decoder as described above

 * `decoder_t::init_decoder(decoder_t*, argc, argv)` <br>
To Initialize the Specified Decoder

 * `decoder_t::get_in_file(decoder_t*)` <br>
Get Input File Pointer

 * `decoder_t::get_in_file_name(decoder_t*)` <br>
Get Input File Name

 * `decoder_t::get_out_file(decoder_t*)` <br>
Get Input File Pointer

 * `decoder_t::get_out_file_name(decoder_t*)` <br>
Get Input File Name

 * `decoder_t::decode(decoder_t*)` <br>
Decode All Data in INPUT File and print output in OUTPUT File

 * `decoder_t::destroy(decoder_t*)` <br>
Destroy the decoder and release all memory

## Options

    -i, --input               Input File with Path
    -o, --output              Output File with Path

