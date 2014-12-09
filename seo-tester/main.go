package main

import (
	"bytes"
	"io"
	"log"
	"os"
	"os/exec"
	"time"

	"proto/seo"

	"github.com/golang/protobuf/proto"
)

func main() {
	// cmd := exec.Command()
	cmd := exec.Command("/bin/bash", "-c",
		"bin/seo -logtostderr -v 1 "+
			"-blacklisted_domains config/blacklisted-domains "+
			"-locales_dir_path /cef/out/Release/locales "+
			"-resources_dir_path /cef/out/Release")
	cmd.Stderr = os.Stderr

	stdin, err := cmd.StdinPipe()
	if err != nil {
		log.Fatal(err)
	}

	stdout, err := cmd.StdoutPipe()
	if err != nil {
		log.Fatal(err)
	}

	log.Println("running renderer...")
	if err := cmd.Start(); err != nil {
		log.Fatal(err)
	}

	go requester(stdin)
	go reader(stdout)

	log.Println("waiting for renderer...")
	if err := cmd.Wait(); err != nil {
		log.Fatal(err)
	}

	log.Println("renderer exited!")
}

func requester(stdin io.WriteCloser) {
	for i := 0; i < 1; i++ {
		log.Println("Sending", i + 1)

		request := &proto_seo.Request{
			Id:      proto.Uint64(uint64(i + 1)),
			Url:     proto.String("http://laovejaverde.es/"),
			Command: proto_seo.Request_GET_SOURCE_CODE.Enum(),
		}
		if err := sendRequest(stdin, request); err != nil {
			log.Fatal(err)
		}

		time.Sleep(3 * time.Second)
	}

	request := &proto_seo.Request{
		Command: proto_seo.Request_EXIT.Enum(),
	}
	if err := sendRequest(stdin, request); err != nil {
		log.Fatal(err)
	}
	
	log.Println("all messages sent!")
}

func sendRequest(stdin io.WriteCloser, request *proto_seo.Request) error {
	log.Println("send request; command:", request.Command)
	data, err := proto.Marshal(request)
	if err != nil {
		return err
	}

	size := proto.EncodeVarint(uint64(proto.Size(request)))
	if _, err := stdin.Write(size); err != nil {
		return err
	}

	if _, err := stdin.Write(data); err != nil {
		return err
	}

	return nil
}

func reader(stdout io.ReadCloser) {
	for {
		log.Println("waiting for responses...")

		// We cannot have lengths greater than uint64, so read only 8 the first 8 bytes
		buffer := make([]byte, 8)
		if _, err := stdout.Read(buffer); err != nil {
			if err == io.EOF {
				return
			}

			log.Fatal(err)
		}

		size, consumed := proto.DecodeVarint(buffer)

		reader := io.MultiReader(bytes.NewReader(buffer[consumed:]), stdout)
		data := make([]byte, size)
		if _, err := io.ReadFull(reader, data); err != nil {
			log.Fatal(err)
		}

		response := &proto_seo.Response{}
		if err := proto.Unmarshal(data, response); err != nil {
			log.Fatal(err)
		}

		log.Println("id:", response.GetId())
		log.Println("status:", response.GetStatus())
		if response.GetStatus() == proto_seo.Response_OK {
			log.Println("source code start:", response.GetSourceCode()[:80])
		}
		if response.GetStatus() == proto_seo.Response_REDIRECT {
			log.Println("redirect url:", response.GetUrl())
		}
	}
}
