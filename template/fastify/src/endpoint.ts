import { FastifyInstance } from "fastify";

import { homeRoute } from "./routes/home";

export function Endpoint(fastify: FastifyInstance) {
  fastify.register(homeRoute);
}
