import {useEffect, useState} from 'react';
import axios from 'axios';
import {response} from "express";

const apiClient = axios.create({
    baseURL: 'http://localhost:8082',
    withCredentials: false,
    headers: {
        Accept: 'application/json',
        'Content-Type': 'application/json'
    }
});


function Compiler() {
    const [code, setCode] = useState("");
    const [output, setOutPut] = useState("");
    const sendCode = () => {
        apiClient.post('/compile', code)
            .then(response => {
                setOutPut(response);
            })
            .catch(error => {
                console.log('Error', error);
            });
    };


    return (
        <div className="Compiler">
            <div className={"input"}>
                <header>Choose the language you want to compile from:<header>C++</header>
                    {/*    Have a drop-down menu here, but start with C++*/}
                    <textarea
                        placeholder={"Your code..."}
                        onChange={(event) => {
                            setCode(event.target.value)
                        }}></textarea>
                </header>
            </div>
            <button onClick={sendCode}>Compile and Run</button>
            <div className={"output"}>
                <header>Output:

                    <textarea>{{ output }}</textarea>
                </header>
            </div>
        </div>
    );
}

export default Compiler;
