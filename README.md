Ciphertext is a mail encrypted using OTP, 
where A, C are those used into glibc rand() function.
The mail follows the MAC OS X Mail application header in Italian

Exploiting the first 4 characters of the header 'Da: ' you can retrieve
every single char of the mail, thus decrypting the mail.
