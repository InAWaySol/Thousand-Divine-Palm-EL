// SPDX-License-Identifier: MIT
pragma solidity ^0.8.24;

interface IERC20 {
    function balanceOf(address account) external view returns (uint256);
    function approve(address spender, uint256 amount) external returns (bool);
    function transfer(address recipient, uint256 amount) external returns (bool);
}

interface ISwapRouter {
    struct ExactInputSingleParams {
        address tokenIn;
        address tokenOut;
        uint24 fee;
        address recipient;
        uint256 deadline;
        uint256 amountIn;
        uint256 amountOutMinimum;
        uint160 sqrtPriceLimitX96;
    }

    function exactInputSingle(
        ExactInputSingleParams calldata params
    ) external payable returns (uint256 amountOut);
}

contract UniVault {

    // =========================
    // HARD-CODED ROLES
    // =========================

    address public constant EXECUTOR =
        0xAb8483F64d9C6d1EcF9b849Ae677dD3315835cb2;

    address public constant TREASURY =
        0x5B38Da6a701c568545dCfcB03FcB875f56beddC4;

    ISwapRouter public constant ROUTER =
        ISwapRouter(0x4B20993Bc481177ec7E8f571ceCaE8A9e22C02db);

    // =========================
    // MODIFIERS
    // =========================

    modifier onlyExecutor() {
        require(msg.sender == EXECUTOR, "NOTEXECUTOR");
        _;
    }

    modifier onlyTreasury() {
        require(msg.sender == TREASURY, "NOTTREASURY");
        _;
    }

    // =========================
    // SWAP
    // =========================

    function swap(
        address tokenIn,
        address tokenOut,
        uint24 fee,
        uint256 amountIn,
        uint256 minOut
    ) external onlyExecutor returns (uint256 amountOut) {

        IERC20 token = IERC20(tokenIn);

        require(
            token.balanceOf(address(this)) >= amountIn,
            "INSUFFICIENT_BALANCE"
        );

        require(
            token.approve(address(ROUTER), amountIn),
            "APPROVE_FAILED"
        );

        ISwapRouter.ExactInputSingleParams memory params =
            ISwapRouter.ExactInputSingleParams({
                tokenIn: tokenIn,
                tokenOut: tokenOut,
                fee: fee,
                recipient: address(this),
                deadline: block.timestamp,
                amountIn: amountIn,
                amountOutMinimum: minOut,
                sqrtPriceLimitX96: 0
            });

        amountOut = ROUTER.exactInputSingle(params);
    }

    // =========================
    // WITHDRAW TOKEN
    // =========================

    function withdrawToken(
        address token,
        uint256 amount
    ) external onlyTreasury {

        require(
            IERC20(token).transfer(TREASURY, amount),
            "TRANSFER_FAILED"
        );
    }

    function withdrawAll(
        address token
    ) external onlyTreasury {

        IERC20 erc = IERC20(token);

        uint256 bal = erc.balanceOf(address(this));

        require(
            erc.transfer(TREASURY, bal),
            "TRANSFER_FAILED"
        );
    }

    // =========================
    // WITHDRAW ETH
    // =========================

    function withdrawETH() external onlyTreasury {

        (bool success,) =
            payable(TREASURY).call{value: address(this).balance}("");

        require(success, "ETH_TRANSFER_FAILED");
    }

    receive() external payable {}
}