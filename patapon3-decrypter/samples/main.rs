extern crate camellia_rs;
extern crate clap;
extern crate indicatif;

use camellia_rs::{Block, CamelliaCipher};
use clap::{App, Arg};
use indicatif::{ProgressBar, ProgressStyle};
use std::fs::File;
use std::io::{Read, Write};

fn encrypt(key: &[u8], data: &mut [u8], decrypt: bool, pb: Option<&ProgressBar>) {
    let block_size = 16;
    let len = data.len();
    let pad_len = if len % block_size != 0 {
        block_size - (len % block_size)
    } else {
        0
    };

    let mut padded_data = Vec::with_capacity(len + pad_len);
    padded_data.extend_from_slice(data);
    padded_data.resize(len + pad_len, 0);

    let cipher = CamelliaCipher::new(key).unwrap();
    let mut buf = Block::default();

    for i in (0..len + pad_len).step_by(block_size) {
        buf.bytes.copy_from_slice(&padded_data[i..(i + block_size)]);
        if buf.bytes.iter().all(|&x| x == 0) {
            // The block is all zero, so we don't need to decrypt/encrypt it
            continue;
        }
        if decrypt {
            cipher.decrypt(&mut buf);
        } else {
            cipher.encrypt(&mut buf);
        }
        padded_data[i..(i + block_size)].copy_from_slice(&buf.bytes);
        if let Some(p) = pb {
            p.inc(1);
        }
    }
    data.copy_from_slice(&padded_data);
}

fn main() {
    let matches = App::new("Patapon 3 Encryptor/Decryptor")
        .version("0.1")
        .author("efonte")
        .about("Encrypts or decrypts Patapon 3 files using Camellia cipher")
        .arg(
            Arg::with_name("input")
                .help("Sets the input file to use")
                .required(true)
                .index(1),
        )
        .arg(
            Arg::with_name("output")
                .help("Sets the output file to use")
                .required(true)
                .index(2),
        )
        .arg(
            Arg::with_name("decrypt")
                .short('d')
                .long("decrypt")
                .takes_value(false)
                .help("Decrypt the input file instead of encrypting it"),
        )
        .arg(
            Arg::with_name("progress")
                .short('p')
                .long("progress")
                .takes_value(false)
                .help("Show progress bar"),
        )
        .get_matches();

    let input_file = matches.value_of("input").unwrap();
    let output_file = matches.value_of("output").unwrap();
    let decrypt = matches.is_present("decrypt");
    let show_progress = matches.is_present("progress");

    let mut input = File::open(input_file).expect("Failed to open input file");
    let mut input_data = Vec::new();
    input
        .read_to_end(&mut input_data)
        .expect("Failed to read input file");

    let key = b"SVsyE56pniSRS9dIPTiE8ApDaUnN0AEa"; // Thanks to Nemoumbra and Owocek
    let mut pb = None;
    if show_progress {
        pb = Some(ProgressBar::new((input_data.len() as u64) / 16));
        pb.as_mut().unwrap().set_style(
            ProgressStyle::default_bar()
                .template(
                    "{spinner:.green} [{elapsed_precise}] [{bar:40.cyan/blue}] {percent}% ({eta})",
                )
                .unwrap()
                .progress_chars("#>-"),
        );
    }

    encrypt(
        key,
        &mut input_data,
        decrypt,
        pb.as_ref().filter(|_| show_progress),
    );

    let mut output = File::create(output_file).expect("Failed to create output file");
    output
        .write_all(&input_data)
        .expect("Failed to write output file");

    if let Some(p) = pb.as_ref() {
        p.finish_with_message("Done");
    }
}
