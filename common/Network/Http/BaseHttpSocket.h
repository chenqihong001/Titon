#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/basic_stream.hpp>
namespace Titan::Net::Http {

	using IoContextHttpSocket = boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::io_context::executor_type, boost::beast::unlimited_rate_policy>;
	// boost::asio::ip::tcp 协议：TCP
	// boost::asio::io_context::executor_type io_context默认执行器
	// boost::beast::unlimited_rate_policy 流量控制：无限制
	// template<class Protocol,class Executor = net::any_io_executor,class RatePolicy = unlimited_rate_policy>
	// Boost 的Executor执行器
	// 1.需要网络IO->io_context::executor_type
	// 2.需要线程安全->strand
	// 3.需要并行计算->thread_pool::executor
	// 4.需要动态行为->any_io_executor


	namespace Impl {

		class BoostBeastSocketWrapper : public IoContextHttpSocket {
		public:
			using IoContextHttpSocket::IoContextHttpSocket;
			
			bool is_open()const {
				return socket().is_open();
				// basic_stream 内部的socket()函数，
			}

			void close() {
				IoContextHttpSocket::close();
			}

			/// <summary>
			/// 用于部分或完全关闭套接字通信方向的函数
			/// </summary>
			/// <param name="what">
			/// 1.shutdown_receive 关闭接收端，不再读取数据
			/// 2.shutdown_send 关闭发送端，不再发送数据
			/// 3.shutdown_both 同时关闭接收和发送
			/// </param>
			/// <param name="shutdownError"></param>
			void shutdown(boost::asio::socket_base::shutdown_type what, boost::system::error_code& shutdownError)
			{
				socket().shutdown(what, shutdownError);
			}
			// close 完全关闭连接（物理层）立即释放系统资源 对端检测到连接断开
			// shutdown 关闭通信方向（逻辑层）不立即释放套接字 对端会收到EOF或错误

			/// <summary>
			/// 异步等待套接字状态变化的函数
			/// </summary>
			/// <typeparam name="WaitHandlerType"></typeparam>
			/// <param name="type">
			/// 1.wait_read 等待套接字可读
			/// 2.wait_write 等待套接字可写
			/// 3.wait_error 等待套接字错误状态
			/// </param>
			/// <param name="handler"></param>
			template<typename WaitHandlerType>
			void async_wait(boost::asio::socket_base::wait_type type, WaitHandlerType&& handler)
			{
				socket().async_wait(type, std::forward<WaitHandlerType>(handler));
			}
		};


	}




}