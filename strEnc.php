<?php
/**
 * strEnc.php
 * 此段程序主要用于接收客户端发送来的用于加密的数据串和加/解密选项，并传回加密过程与结果。
 * $inStr为接收的数据串
 * $key为接收的密钥
 * $en为加/解密选项，值为"enc"表示加密，值为"dec"表示解密
 *
 * 在取出客户端发送的信息后，将信息组装为系统命令，格式为：./aesEnc 输入串 密钥 加密选项（e或d）
 * 执行完该条命令后，./aesEnc会输出三行结果，第一行为KeyExpansion的过程，第二行为加/解密的过程，第三行为输出串
 *
 * 程序将输出的结果按行储存在$retArr数组中，并将其组装为json格式数据，然后发送回客户端
 *
 */
header("Content-type:text/html;charset=utf-8");
    $inStr = $_POST['inStr'];
    $key = $_POST['key'];
    $en = $_POST['en'];

    $command = './aesEnc '.$inStr.' '.$key.' '.$en[0];

    exec($command, $retArr);

    $json_arr = array("outStr" => $retArr[2], "procedure" => $retArr[1], "keyEx" => $retArr[0]);
    $json_obj = json_encode($json_arr);
    echo $json_obj;