import React from 'react';
import { Box, Button, Flex, Image, Link, Spacer, Img} from '@chakra-ui/react';

import GUI from "./AAP.png";

const NavBar = ({accounts, setAccounts}) => {
    const isConnected = true;//Boolean(accounts[0]);

    async function connectAccount(){
        if(window.ethereum) {
            const accounts = await window.ethereum.request({
                method: "eth_requestAccounts",
            });
        setAccounts(accounts);
        }
    }

    return(
       
        

        <Flex justify="center" align="center"  margin="0" position="absolute" >
        
            
            <Box boxSize='auto' height="center"     >
                <Image src={GUI} />
                
            </Box>
           
            {/*Right Side - Sections and Connect*/}
            <Flex>
            <Box margin="0 0px"></Box>
            
           
            
      


        </Flex>
</Flex> 




    );
};

export default NavBar;