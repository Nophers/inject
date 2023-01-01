import Fastify, { FastifyInstance } from "fastify";
import cors from "@fastify/cors";

import { Endpoint } from "./endpoint";

/**
 * Server class to initialize the server
 * @class Server
 * @constructor
 * @param {FastifyInstance} fastify - Fastify instance
 * @param {FastifyRequest} request - Fastify request
 * @param {FastifyReply} reply - Fastify reply
 * @param {RoutesOptions} opts - Routes options
 * @returns {void}
 */
class Server {
  private fastify: FastifyInstance;

  constructor() {
    this.fastify = Fastify({
      logger: true,
    });
  }

  public async start(): Promise<void> {
    try {
      await this.fastify.listen({ port: 2000 });
      console.log(`Server is listening on Port ${2000}`);
    } catch (err) {
      this.fastify.log.error(err);
      process.exit(1);
    }
  }

  public addCors(): void {
    this.fastify.register(cors, {
      origin: "http://localhost:3000",
      methods: ["GET", "POST"],
      allowedHeaders: ["Content-Type"],
    });
  }
  public async registerRoutes(): Promise<void> {
    Endpoint(this.fastify);
  }
}

export function startServer() {
  const server = new Server();
  server.registerRoutes();
  server.addCors();
  server.start();
}
