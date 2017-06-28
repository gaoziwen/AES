<?php
/**
 * shiftEnc.php
 * 此段程序主要用于接收客户端发送来的用于计算ShiftRows的输入串和密钥，并传回计算后的串。
 * $inStr为接收的串
 *
 * 在取出客户端发送的信息后，将信息组装为系统命令调用可执行文件calAes，格式为：./calAes 输入串 1
 * 执行完该条命令后，./calAes会打印一行输出串
 *
 * 程序将输出的结果按行储存在$retArr数组中，并将其组装为json格式数据，然后发送回客户端
 *
 */
header("Content-type:text/html;charset=utf-8");
$inStr = $_POST['inStr'];

$command = './calAes '.$inStr.' 1';

exec($command, $retArr);

$json_arr = array("shiftOut" => $retArr[0]);
$json_obj = json_encode($json_arr);
echo $json_obj;